#pragma once

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "SpriteRenderer.h"

#include <vector>

struct Animation
{
	std::string name;

	int index = 0;
	int frames = 0;
	int speed = 0;
	bool looping = true;

	Animation() = default;
	Animation(std::string n, int i, int f, int s, bool loop = true)
	{
		name = n;
		index = i;
		frames = f;
		speed = s;
		looping = loop;
	}

	std::string ToString()
	{
		std::string str = std::string(name) + ',' + std::to_string(index) + ',' + std::to_string(frames) + ',' + std::to_string(speed);
		return str;
	}

	static Animation FromString(std::string value)
	{
		Animation animation;

		vector<std::string> values;
		std::string currentVal;

		for (char& c : value)
		{
			if (c == ',')
			{
				values.push_back(currentVal);
				currentVal.clear();
			}
			else
			{
				currentVal.push_back(c);
			}
		}

		values.push_back(currentVal);

		animation.name = values[0];
		animation.index = std::stoi(values[1]);
		animation.frames = std::stoi(values[2]);
		animation.speed = std::stoi(values[3]);

		return animation;
	}
};

class Animator : public SpriteRenderer
{
public:

	Animator(const char* path, Vector2 size);

	void Init() override;
	void Update() override;

	void AddNewAnimation(std::string animName, int index, int frames, int speed, bool loop = true);
	void AddNewAnimation(Animation& animation);

	void PlayAnimation(const char* animName);
	void PlayAnimation(int index);

	bool FinishedAnimation(const char* animName);
	bool FinishedAnimation(int index);

	int animIndex = 0;
	std::vector<Animation> animations;

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName, currentPath, size.ToString(), animsString);
	}

private:

	int GetAnimIndex(const char* animName);

	const char*		currentAnimationName;

	int				frames = 1;
	int				speed = 100; //delay between frames in milliseconds
	bool			looping = true;

	int				ticksOffset;

	std::string		animsString;
};

#endif