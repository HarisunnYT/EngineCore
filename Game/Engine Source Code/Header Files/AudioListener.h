#pragma once

#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#pragma warning (disable : 4005)

#include "Audio.h"
#include "EngineCore.h"

class AudioListener : public Component
{
public:

	AudioListener();
	~AudioListener();

	void Init() override;
	void Update() override;

	std::unique_ptr<DirectX::AudioEngine> audioEngine;

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName);
	}

	bool audioEnabled = true;

};

#endif