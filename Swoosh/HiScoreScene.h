#pragma once

#pragma once
#include "ActivityController.h"
#include <SFML\Graphics.hpp>
#include "TextureLoader.h"
#include "Particle.h"
#include "Button.h"
#include "ResourcePaths.h"
#include "SaveFile.h"
#include "Collision.h"

#include "SlideIn.h"

#include <iostream>

using namespace swoosh;

class HiScoreScene : public Activity {
private:
  sf::Texture * meteorBig, * meteorMed, * meteorSmall, * meteorTiny, * btn;

  std::vector<particle> meteors;
  button goback;

  sf::Font   font;
  sf::Text   text;

  save hiscore;

  int lives;

  float screenDiv;
  float screenMid;
  float screenBottom;

  Timer waitTime;
  double scrollOffset;

public:
  HiScoreScene(ActivityController& controller) : Activity(controller) {
    font.loadFromFile(GAME_FONT);
    text.setFont(font);
    text.setFillColor(sf::Color::White);

    btn = loadTexture(BLUE_BTN_PATH);
    goback.sprite = sf::Sprite(*btn);
    goback.text = "Return";

    meteorBig = loadTexture(METEOR_BIG_PATH);
    meteorMed = loadTexture(METEOR_MED_PATH);
    meteorSmall = loadTexture(METEOR_SMALL_PATH);
    meteorTiny = loadTexture(METEOR_TINY_PATH);

    screenBottom = getController().getWindow().getSize().y;
    screenMid = getController().getWindow().getSize().x / 2.0;
    screenDiv = getController().getWindow().getSize().x / 4.0;

    hiscore.loadFromFile(SAVE_FILE_PATH);

    if (hiscore.empty()) {
      hiscore.writeToFile(SAVE_FILE_PATH);
      hiscore.loadFromFile(SAVE_FILE_PATH);
    }

    waitTime.start();
    scrollOffset = 0;
  }

  virtual void onStart() {
   
  }

  virtual void onUpdate(double elapsed) {
    goback.update(getController().getWindow());

    if (goback.isClicked) {
      getController().queuePop<ActivityController::Segue<SlideIn>>();
    }

    // After 3 seconds, scroll up
    if (waitTime.getElapsed().asSeconds() > 3) {

      // If the scroll offset is greater than the height of all drawn scores
      if (scrollOffset > 200 + (hiscore.names.size() * 100)) {
        // We hit them all, reset
        scrollOffset = 0;
        waitTime.reset();
      }
      else {
        scrollOffset += 100.0 * elapsed;
      }
    }

    for (auto& m : meteors) {
      sf::Vector2f prevPos = m.pos;
      m.pos += sf::Vector2f(m.speed.x * elapsed, m.speed.y * elapsed);
      m.sprite.setPosition(m.pos);
      m.sprite.setRotation(m.pos.x);
    }
  }

  virtual void onLeave() {

  }

  virtual void onExit() {
  }

  virtual void onEnter() {
    for (int i = 50; i > 0; i--) {
      int randNegativeX = rand() % 2 == 0 ? -1 : 1;
      int randNegativeY = rand() % 2 == 0 ? -1 : 1;

      int randSpeedX = rand() % 40;
      randSpeedX *= randNegativeX;

      int randSpeedY = rand() % 40;
      randSpeedY *= randNegativeY;

      particle p;

      int randTexture = rand() % 4;

      switch (randTexture) {
      case 0:
        p.sprite = sf::Sprite(*meteorBig);
        break;
      case 1:
        p.sprite = sf::Sprite(*meteorMed);
        break;
      case 2:
        p.sprite = sf::Sprite(*meteorSmall);
        break;
      default:
        p.sprite = sf::Sprite(*meteorTiny);
      }

      p.pos = sf::Vector2f(rand() % getController().getWindow().getSize().x, rand() % getController().getWindow().getSize().y);
      p.sprite.setPosition(p.pos);

      p.speed = sf::Vector2f(randSpeedX, randSpeedY);
      meteors.push_back(p);
    }
  }

  virtual void onResume() {
  }

  virtual void onDraw(sf::RenderTexture& surface) {
    for (auto& m : meteors) {
      surface.draw(m.sprite);
    }

    text.setFillColor(sf::Color::Yellow);
    text.setPosition(sf::Vector2f(screenMid, 100));
    text.setString("Hi Scores");
    text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
    surface.draw(text);

    text.setFillColor(sf::Color::White);
    
    for (int i = 0; i < hiscore.names.size(); i++) {
      std::string name = hiscore.names[i];
      int score = hiscore.scores[i];

      text.setString(name);
      text.setPosition(sf::Vector2f(screenDiv, 200 + (i*100) - scrollOffset));
      text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
      surface.draw(text);

      text.setString(std::to_string(score));
      text.setPosition(sf::Vector2f(screenDiv * 3, 200 + (i*100) - scrollOffset));
      text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
      surface.draw(text);
    }

    text.setFillColor(sf::Color::Black);
    goback.draw(surface, text, screenMid, screenBottom - 40);

    surface.draw(text);
  }

  virtual void onEnd() {
  }

  virtual ~HiScoreScene() { delete btn; }
};