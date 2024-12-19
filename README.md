# Naval Combat Game

A 2D naval combat game featuring historical ship battles between pirates and the Royal Navy. Built with Raylib graphics library.

## 🎮 Game Overview

Players control a Royal Navy vessel in combat against AI-controlled pirate ships. The game features realistic naval combat mechanics including:

- Ship steering and speed control
- Manual cannon loading and firing system
- Dynamic ocean environment
- Damage and sinking mechanics
- Historical ship designs and flags

## 🔧 Technical Features

### Ship Physics
- Realistic momentum-based movement
- Wave effects on ships
- Collision detection
- Damage system with health tracking

### Combat System
- Manual cannon loading workflow
- Projectile physics
- Range-based accuracy
- Multiple cannon positions (port/starboard)

### AI Behavior
- Intelligent enemy ship maneuvering
- Combat distance management
- Strategic firing patterns
- Patrol and engage behaviors

### Graphics
- Animated ocean waves
- Dynamic ship rendering
- Particle effects for cannon fire
- UI elements for ship control

## 🎯 Controls

- **Ship Steering**: Mouse wheel control
- **Speed Control**: Throttle lever
- **Cannon Loading**:
  1. Drag cannonball to cannon
  2. Use ramrod to load
  3. Ready to fire
- **Fire**: Press fire button when cannons are loaded

## 🛠️ Building the Project

### Prerequisites
- C++ compiler with C++11 support
- Raylib library
- CMake build system

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
