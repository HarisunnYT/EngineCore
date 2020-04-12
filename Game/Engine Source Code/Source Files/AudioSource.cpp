#include "AudioSource.h"

std::string AudioSource::componentName = "audiosource";

AudioSource::AudioSource()
{
	sounds = vector<AudioClip>();
}

AudioSource::~AudioSource()
{
	for (auto& sound : sounds)
	{
		delete sound.sound;
	}
}

void AudioSource::Play(const wchar_t* path)
{
	if (!EngineCore::audioListener->audioEnabled)
		return;

	soundEffect = std::make_unique<DirectX::SoundEffect>(EngineCore::audioListener->audioEngine.get(), path);
	AudioClip clip = AudioClip(path, soundEffect.get(), SDL_GetTicks() + soundEffect->GetSampleDurationMS());
	sounds.push_back(clip);
	sounds.back().sound->Play();
}

std::string AudioSource::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(AudioSource::componentName);
	}

	return ss.str();
}

bool AudioSource::TryParse(std::string value, Entity* entity)
{
	std::string name;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name);
	}

	if (name == AudioSource::componentName)
	{
		entity->AddComponent<AudioSource>();

		return true;
	}

	return false;
}
