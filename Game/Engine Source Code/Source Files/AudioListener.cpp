#include "..\Header Files\AudioListener.h"

std::string AudioListener::componentName = "audiolistener";

AudioListener::AudioListener()
{
	EngineCore::audioListener = this;
}

AudioListener::~AudioListener()
{
}

void AudioListener::Init()
{
	auto hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		std::cout << "Failed" << std::endl;
	}

	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
#endif

	audioEngine = std::make_unique<DirectX::AudioEngine>(eflags);
}

void AudioListener::Update()
{
	audioEngine->Update();
}

std::string AudioListener::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(AudioListener::componentName);
	}

	return ss.str();
}

bool AudioListener::TryParse(std::string value, Entity* entity)
{
	std::string name;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name);
	}

	if (name == AudioListener::componentName)
	{
		entity->AddComponent<AudioListener>();

		return true;
	}

	return false;
}
