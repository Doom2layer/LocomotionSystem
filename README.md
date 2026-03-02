# HorrorZone - Advanced Locomotion System

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.0+-blue.svg)](https://www.unrealengine.com/)
[![C++](https://img.shields.io/badge/C++-17-orange.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-See%20LICENSE-green.svg)](LICENSE)

A comprehensive Unreal Engine 5 C++ technical showcase featuring an advanced locomotion system, AI combat mechanics, weapon systems, and custom Control Rig implementations. Built with production-quality architecture and clean code practices.

![Project Banner](Documentation/banner.png)
*Advanced locomotion with procedural animation and team-based AI combat*

---

## 🎯 Project Overview

**HorrorZone** demonstrates professional-grade game development with Unreal Engine 5, showcasing:

- 🚶 **Advanced Locomotion System** - Data-driven movement with distance matching
- 🤖 **AI Combat System** - Team-based behavior with custom Behavior Tree tasks  
- 🔫 **Weapon System** - Firearms, melee, and combo mechanics
- 🦴 **Control Rig C++** - Custom IK units for procedural animation
- 🎬 **Animation System** - Thread-safe, layered blending with overrides
- 👥 **Team System** - Configurable team affiliations and combat
- 🎨 **UI System** - Optimized world-space and screen-space widgets
- ⚡ **Performance Optimized** - Timer-based updates, caching, threading

---

## 📚 Documentation

Comprehensive documentation is provided in three detailed guides:

| Document | Description | Target Audience |
|----------|-------------|-----------------|
| **[PROJECT_DOCUMENTATION.md](PROJECT_DOCUMENTATION.md)** | Complete system overview, architecture, and features | All readers |
| **[TECHNICAL_IMPLEMENTATION.md](TECHNICAL_IMPLEMENTATION.md)** | Implementation details, code examples, and deep dives | Developers |
| **[PORTFOLIO_SHOWCASE.md](PORTFOLIO_SHOWCASE.md)** | Portfolio presentation and interview preparation | Job seekers |

---

## 🎥 Demo Video

[![Demo Video](Documentation/video_thumbnail.png)](https://youtu.be/YOUR_VIDEO_ID)

**Features Demonstrated:**
- Smooth locomotion transitions (Walk, Jog, Sprint, Crouch)
- Weapon switching and combat mechanics
- AI team-based combat (Red vs Green vs Blue)
- Procedural IK foot placement
- Dynamic UI and crosshair system

---

## ✨ Key Features

### Locomotion System
- **Data-Driven Configuration** - All animation sets in DataTables
- **Distance Matching** - Realistic foot placement based on actual distance
- **Turn-In-Place** - Smooth rotation animations when stationary
- **Multiple Movement States** - Walk, Jog, Sprint, Crouch
- **Rotation Modes** - Strafing and Forward-Facing
- **Animation Overrides** - Per-body-part blending system

### Weapon System
- **Multiple Weapon Types** - Firearms (rifle, pistol, shotgun) and melee
- **Slot-Based Inventory** - Easy weapon switching
- **Ammo Management** - Magazine and spare ammo tracking
- **Melee Combos** - Save attack system for fluid combat
- **Per-Weapon Settings** - Custom camera, animations, and behavior
- **Dynamic Attachment** - Proper weapon-to-character attachment

### AI System
- **Behavior Tree Implementation** - Custom tasks and services
- **Team Affiliation** - Configurable enemy/ally relationships
- **Combat Behavior** - Fire duration control and target prioritization
- **Perception System** - Distance-based target detection
- **Patrol Behavior** - Random location navigation
- **Smart Targeting** - Team-aware enemy selection

### Control Rig System
- **Custom C++ Units** - FRigUnit_SetFinalLegIK for procedural IK
- **Automatic Bone Chain Resolution** - Thigh → Calf → Foot hierarchy
- **Scale-Aware Calculations** - Proper handling of non-uniform scaling
- **Left/Right Symmetry** - Mirrored axis for both legs
- **Production Quality** - Implementation mirrors Epic's engine code

### UI System
- **World-Space Widgets** - Health bars and team indicators
- **Sense Rules** - Optimized visibility based on distance/angle
- **Dynamic HUD** - Weapon info, ammo, crosshair
- **Performance Optimized** - Timer-based updates, not every frame

---

## 🏗️ Architecture

### Component Hierarchy

```
ACharacter (Unreal Engine Base)
    ↓
ALocomotionSystem_CharacterBase
    ├── ULocomotionSystem        (Movement & Animation)
    ├── UWeaponSystem             (Inventory & Combat)
    ├── UActorProfileSystem       (Health & Teams)
    ├── UMontageHelper            (Animation Management)
    └── UUserInterfaceSystem      (HUD & Widgets)
    ↓
    ├── ALocomotionSystem_PlayerBase (Player-Controlled)
    └── ALocomotionSystem_NPCBase    (AI-Controlled)
```

### Key Design Patterns

- **Component-Based Architecture** - Modular, reusable systems
- **Data-Driven Design** - Configuration through DataTables
- **Delegate-Based Communication** - Loose coupling between systems
- **Interface-Driven** - IWeaponInterface, IInventoryItemInterface, ILocomotionInterface
- **Observer Pattern** - Event broadcasting for state changes

---

## 🛠️ Technical Highlights

### Advanced C++ Features

```cpp
// Component-based architecture
UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
ULocomotionSystem* LocomotionSystem;

// Delegate system for loose coupling
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSlotChanged, int32, NewSlot);

// Interface implementation
class AWeaponBase : public AActor, 
                    public IWeaponInterface, 
                    public IInventoryItemInterface
{
    // Multiple interface support
};

// Data-driven configuration
UPROPERTY(EditAnywhere, BlueprintReadWrite)
UDataTable* AnimsetDataTable;
```

### Control Rig Implementation

```cpp
// Custom Control Rig unit
USTRUCT(meta=(DisplayName="Set Final Foot IK"))
struct FRigUnit_SetFinalLegIK : public FRigUnitMutable
{
    GENERATED_BODY()
    
    UPROPERTY(meta = (Input))
    TArray<FRigElementKey> FootItems;
    
    RIGVM_METHOD()
    virtual void Execute() override;
};
```

### Thread-Safe Animation

```cpp
// Proper thread separation
void NativeUpdateAnimation(float DeltaSeconds) override
{
    // Game thread - component access
}

void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override
{
    // Worker thread - calculations only
}
```

---

## 📂 Project Structure

```
Source/HorrorZone/MainContent/
├── LocomotionSystem/
│   ├── Character/              # Player & NPC character classes
│   ├── Components/
│   │   ├── SubSystems/         # LocomotionSystem, WeaponSystem, etc.
│   │   └── Helper/             # MontageHelper, utilities
│   ├── AI/
│   │   ├── BTTask/             # Custom BT tasks (Attack, SetFocus, etc.)
│   │   ├── BTService/          # Custom BT services
│   │   └── Controller/         # AI Controller implementation
│   ├── Animation/
│   │   ├── AnimInstance        # Custom animation instance
│   │   └── Animation Notifies/ # Custom anim notifies
│   ├── Weapons/                # Weapon actor implementations
│   │   ├── WeaponBase          # Base weapon class
│   │   └── Melee/              # Melee weapon types
│   ├── Data/                   # Enums, structs, configs
│   ├── Widget/                 # UI widget classes
│   └── Utilities/              # Helper functions
└── ControlRig/                 # Custom Control Rig units
    ├── FRigUnit_SetFinalLegIK
    ├── FRigUnit_RotateBoneInPlace
    └── FRigUnit_RotateAroundPoint
```

---

## 🚀 Getting Started

### Prerequisites

- Unreal Engine 5.0 or later
- Visual Studio 2019/2022 with C++ support
- Windows 10/11

### Building the Project

1. Clone the repository:
```bash
git clone https://github.com/yourusername/HorrorZone.git
```

2. Navigate to project directory:
```bash
cd HorrorZone
```

3. Right-click `HorrorZone.uproject` → Generate Visual Studio project files

4. Open `HorrorZone.sln` in Visual Studio

5. Set build configuration to `Development Editor`

6. Build the solution (Ctrl + Shift + B)

7. Launch from Visual Studio or open `HorrorZone.uproject`

### Quick Start Guide

1. **Test Locomotion:**
   - Play in editor
   - Use WASD for movement
   - Hold Shift to sprint
   - C to crouch
   - Left Ctrl to walk

2. **Test Weapons:**
   - Mouse scroll to switch weapons
   - Left click to fire/attack
   - R to reload
   - Right click to aim (firearms)

3. **Test AI:**
   - Place NPCs in level
   - Set their TeamID in ActorProfileSystem
   - Configure enemies/allies in TeamConfigDataTable
   - Run and observe team-based combat

---

## 📊 Project Statistics

| Metric | Value |
|--------|-------|
| **Total C++ Files** | 100+ (headers + source) |
| **Lines of Code** | ~10,000+ |
| **Custom Components** | 8 major subsystems |
| **Data Structures** | 30+ custom structs |
| **Enums** | 15+ custom enums |
| **BT Tasks** | 4 custom tasks |
| **Control Rig Units** | 3 custom units |
| **Animation Notifies** | 3 custom notifies |
| **Widget Classes** | 6 UI widgets |

---

## 🎓 Learning Resources

This project serves as an educational resource for:

- **Unreal Engine C++ Development** - Production patterns and practices
- **Game System Architecture** - Component-based design
- **Animation Programming** - Locomotion and procedural animation
- **AI Development** - Behavior Trees and perception
- **Control Rig** - Custom C++ unit development
- **Data-Driven Design** - Separating data from logic

---

## 🤝 Use Cases

### As Portfolio Piece
- Demonstrates production-quality code
- Shows architectural thinking
- Proves Unreal Engine mastery
- Exhibits clean code practices

### As Game Foundation
Can be extended for:
- Third-person shooters
- Action RPGs
- Stealth games
- Horror games
- Combat-focused titles

### As Learning Material
- Well-structured for study
- Demonstrates best practices
- Includes advanced concepts
- Comprehensive documentation

---

## 🔧 System Requirements

### Development
- **OS:** Windows 10/11 64-bit
- **CPU:** Quad-core Intel/AMD, 2.5 GHz+
- **RAM:** 16 GB minimum, 32 GB recommended
- **GPU:** DirectX 11/12 compatible with 4GB VRAM
- **Storage:** 100+ GB free space (SSD recommended)

### Runtime
- **OS:** Windows 10/11 64-bit
- **CPU:** Quad-core Intel/AMD
- **RAM:** 8 GB minimum
- **GPU:** DirectX 11 compatible with 2GB VRAM

---

## 📝 Code Quality

### Best Practices Demonstrated

- ✅ **Null Pointer Checks** - Comprehensive validation
- ✅ **Error Handling** - Graceful failures with logging
- ✅ **Memory Management** - Smart pointers (TObjectPtr)
- ✅ **Performance** - Caching, timers, threading
- ✅ **Maintainability** - Clear naming, modular structure
- ✅ **Documentation** - Inline comments, external docs
- ✅ **Extensibility** - Virtual functions, interfaces
- ✅ **SOLID Principles** - Clean architecture

### Code Example

```cpp
void UWeaponSystem::Initialize()
{
    AActor* OwningActor = GetOwner();
    if (!OwningActor)
    {
        UE_LOG(LogTemp, Error, TEXT("WeaponSystem has no owner"));
        return;
    }

    for (const auto& WeaponClass : DefaultWeapons)
    {
        if (WeaponClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = OwningActor;
            SpawnParams.SpawnCollisionHandlingOverride = 
                ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(
                WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams
            );

            if (Weapon)
            {
                // Double-check owner for AI characters
                if (Weapon->GetOwner() == nullptr)
                {
                    Weapon->SetOwner(OwningActor);
                }
                AddWeapon(Weapon);
            }
        }
    }
}
```

---

## 🐛 Known Issues & Limitations

### Current Limitations
- Single-player only (multiplayer not implemented)
- AI limited to basic combat behavior
- Limited weapon variety (foundation in place)

### Future Enhancements
- [ ] Multiplayer support with replication
- [ ] Advanced AI behaviors (flanking, cover)
- [ ] More weapon types and attachments
- [ ] Inventory system expansion
- [ ] Save/load system
- [ ] Advanced IK (hand IK, look-at)

---

## 📄 License

See [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- **Unreal Engine** by Epic Games
- **Control Rig System** documentation and examples
- Community tutorials and resources

---

## 📞 Contact

- **Portfolio:** [Your Website]
- **LinkedIn:** [Your Profile]
- **Email:** [your.email@example.com]
- **GitHub:** [Your Profile]

---

## 🌟 Showcase

### Video Demonstrations

| Feature | Demo Link |
|---------|-----------|
| Locomotion System | [Watch Demo](#) |
| AI Combat | [Watch Demo](#) |
| Weapon System | [Watch Demo](#) |
| Control Rig IK | [Watch Demo](#) |

### Screenshots

![Player Blueprint](Documentation/blueprints/player_blueprint.png)
*Player character component architecture and class defaults*

![Control Rig Bipedal](Documentation/controlrig/controlrig_bipedal.png)
*Control Rig graph for bipedal IK system*

![Control Rig Scorpion](Documentation/controlrig/controlrig_scorpion.png)
*Control Rig graph for scorpion multi-leg IK*

![DataTable Animations](Documentation/datatables/datatable_animations.png)
*Data-driven animation configuration*

![DataTable Teams](Documentation/datatables/datatable_teams.png)
*Team affiliation system setup*

![DataTable Camera](Documentation/datatables/datatable_camera.png)
*Per-weapon camera settings*

---

## 💡 Key Takeaways

This project demonstrates:

1. **Professional C++ Development** - Production-ready code quality
2. **System Architecture** - Modular, maintainable design
3. **Unreal Engine Mastery** - Deep understanding of engine systems
4. **Animation Programming** - Complex locomotion and procedural animation
5. **AI Development** - Behavior Trees and team-based AI
6. **Performance Optimization** - Threading, caching, and profiling
7. **Data-Driven Design** - Designer-friendly configuration
8. **Clean Code Practices** - SOLID principles and best practices

---

## 🎯 Perfect For

- **Job Applications** - Demonstrates professional skills
- **Portfolio Presentations** - Shows technical depth
- **Learning Unreal C++** - Well-documented examples
- **Game Prototypes** - Solid foundation to build upon
- **Technical Interviews** - Discussion material

---

**Built with ❤️ using Unreal Engine 5**

*Last Updated: March 2026*

---

**⭐ Star this repository if you find it useful!**

**📢 Share with others learning Unreal Engine C++!**

**🔔 Watch for updates and new features!**

