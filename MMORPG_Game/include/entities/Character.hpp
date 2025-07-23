#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class CharacterClass {
    WARRIOR,
    MAGE,
    ARCHER
};

class Character {
public:
    Character(const std::string& name, CharacterClass characterClass);
    virtual ~Character() = default;

    // Getters
    const std::string& getName() const { return name; }
    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getStrength() const { return strength; }
    int getDexterity() const { return dexterity; }
    int getIntelligence() const { return intelligence; }

    // Character progression
    void gainExperience(int amount);
    void levelUp();
    void allocateStatPoints(int strength, int dexterity, int intelligence);

    // Combat
    virtual void takeDamage(int amount);
    virtual void heal(int amount);
    bool isAlive() const { return health > 0; }

    // Movement
    void setPosition(float x, float y);
    const sf::Vector2f& getPosition() const { return position; }

protected:
    std::string name;
    CharacterClass characterClass;
    int level;
    int experience;
    int health;
    int maxHealth;
    
    // Base stats
    int strength;
    int dexterity;
    int intelligence;
    
    // Position
    sf::Vector2f position;
    
    // Experience required for next level
    int getExperienceForNextLevel() const;
}; 