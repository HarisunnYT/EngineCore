#pragma once

#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "Audio.h"
#include "EngineCore.h"

struct AudioClip
{
	const wchar_t* path;
	DirectX::SoundEffect* sound;
	size_t time;

	AudioClip(const wchar_t* p, DirectX::SoundEffect* s, size_t t)
	{
		path = p;
		sound = s;
		time = t;
	}
};

class AudioSource : public Component
{
public:

	AudioSource();
	~AudioSource();

	void Play(const wchar_t* path);

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName);
	}

private:

	vector<AudioClip> sounds;
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
};

#endif