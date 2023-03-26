#pragma once
#include <iostream>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class AudioManager
{

public:

	AudioManager() {

		if (!buffer_die.loadFromFile("Resources/Sounds/sfx_keelOverAndDie.wav")) {
			cout << "WARNING: COULD NOT LOAD sfx_keelOverAndDie.wav" << endl;
		}
		if (!buffer_eat.loadFromFile("Resources/Sounds/sfx_nom.wav")) {
			cout << "WARNING: COULD NOT LOAD sfx_nom.wav" << endl;
		}
		if (!buffer_oxygen.loadFromFile("Resources/Sounds/sfx_oxygenate.wav")) {
			cout << "WARNING: COULD NOT LOAD sfx_oxygenate.wav" << endl;
		}
		if (!buffer_splash.loadFromFile("Resources/Sounds/sfx_splash.wav")) {
			cout << "WARNING: COULD NOT LOAD sfx_splash.wav" << endl;
		}
		if (!buffer_damage.loadFromFile("Resources/Sounds/sfx_damage.wav")) {
			cout << "WARNING: COULD NOT LOAD sfx_damage.wav" << endl;
		}
		if (!buffer_eatBonus.loadFromFile("Resources/Sounds/sfx_nomDX.wav")) {
			cout << "WARNING: COULD NOT LOAD sfx_nomDX.wav" << endl;
		}

		sfx_die.setBuffer(buffer_die);
		sfx_eat.setBuffer(buffer_eat);
		sfx_eatBonus.setBuffer(buffer_eatBonus);
		sfx_oxygen.setBuffer(buffer_oxygen);
		sfx_splash.setBuffer(buffer_splash);
		sfx_damage.setBuffer(buffer_damage);
	}

	void play_sfx_die() {
		float newPitch = (float)((rand() % 100 + 1) + 50) / (float)100;
		sfx_die.setPitch(newPitch);
		sfx_die.play();
	}

	void play_sfx_eat() {
		float newPitch = (float)((rand() % 100 + 1) + 50) / (float)100;
		sfx_eat.setPitch(newPitch);
		sfx_eat.play();
	}

	void play_sfx_eatBonus() {
		sfx_eatBonus.play();
	}

	void play_sfx_oxygen() {
		float newPitch = (float)((rand() % 100 + 1) + 50) / (float)100;
		sfx_oxygen.setPitch(newPitch);
		sfx_oxygen.play();
	}

	void play_sfx_splash() {
		float newPitch = (float)((rand() % 100 + 20) + 20) / (float)100;
		sfx_splash.setPitch(newPitch);
		sfx_splash.play();
	}

	void play_sfx_damage() {
		float newPitch = (float)((rand() % 100) + 20) / (float)100;
		sfx_damage.setPitch(newPitch);
		sfx_damage.play();
	}

private:
	sf::SoundBuffer buffer_die;
	sf::SoundBuffer buffer_eat;
	sf::SoundBuffer buffer_eatBonus;
	sf::SoundBuffer buffer_oxygen;
	sf::SoundBuffer buffer_splash;
	sf::SoundBuffer buffer_damage;

	sf::Sound sfx_die;
	sf::Sound sfx_eat;
	sf::Sound sfx_eatBonus;
	sf::Sound sfx_oxygen;
	sf::Sound sfx_splash;
	sf::Sound sfx_damage;
};

