#include "DuneAvatar.h"
//#include "vDuneCore/Public/Avatar/DuneController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
//#include "vDuneCore/Public/Avatar/Interfaces/Pickup/PickupActor.h"
//#include "vDuneCore/Public/Avatar/Interfaces/Pickup/PickupModel.h"
//#include "vDuneCore/Public/Avatar/Interfaces/Observable/ObservableActor.h"
//#include "vDuneCore/Public/Avatar/Interfaces/ViableInteraction.h"
//#include "vDuneCore/Public/Avatar/Interfaces/Tools/AvatarTool.h"
//#include "vDuneCore/Public/Avatar/Interfaces/Tools/MeasureTool.h"
//#include "vDuneCore/Public/Avatar/Interfaces/Menus/AvatarMenu.h"
//#include "vDuneCore/Public/Decorator/UserName.h"
//#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADuneAvatar

ADuneAvatar::ADuneAvatar(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
//, DecoratorBase()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
        
    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    camera_boom_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    camera_boom_->SetupAttachment(RootComponent);
    camera_boom_->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
    camera_boom_->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    follow_camera_ = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    follow_camera_->SetupAttachment(camera_boom_, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    follow_camera_->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

//	collection_sphere_ = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
//	collection_sphere_->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
//	collection_sphere_->SetSphereRadius(200.0f);
//
//    available_tool_.Add(EAvatarTool::None, nullptr);
//	available_tool_.Add(EAvatarTool::MeasureTool, nullptr);
//	available_tool_.Add(EAvatarTool::InspectTool, nullptr);
//	available_tool_.Add(EAvatarTool::BallDropExperiment, nullptr);
//
//    available_mode_.Add(EAvatarMode::Roam, nullptr);
//    available_mode_.Add(EAvatarMode::Menu, nullptr);
//    available_mode_.Add(EAvatarMode::ServerMenu, nullptr);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

//	DecoratorBase::set_decorator_name("Avatar");
//	DecoratorBase::add_decorator<UserName>();
}

void ADuneAvatar::BeginPlay()
{
    Super::BeginPlay();


}

//////////////////////////////////////////////////////////////////////////
// Input

void ADuneAvatar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    UE_LOG(LogTemp, Log, TEXT("Setup Input..."))
    
    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    
    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        
        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADuneAvatar::Move);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADuneAvatar::Look);
    }
    else
    {
        UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
    }

	// VR headset functionality
//	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADuneAvatar::OnResetVR);

//    FInputActionBinding MeasureModeBinding( "MeasureMode", IE_Released );
//    MeasureModeBinding.ActionDelegate.GetDelegateForManualSet().BindLambda( [this]()
//    {
//        this->set_measure_mode();
//    });
//    PlayerInputComponent->AddActionBinding(MeasureModeBinding);
//
//    FInputActionBinding PlaceMarkerBinding( "MeasurePlace", IE_Released );
//    PlaceMarkerBinding.ActionDelegate.GetDelegateForManualSet().BindLambda( [this]()
//    {
//        //if the mode is the measure tool.
//        UMeasureTool * measure_tool = Cast<UMeasureTool>(mode_);
//
//        if (measure_tool)
//            measure_tool->add_marker();
//
//        else
//            UE_LOG(LogClass, Warning, TEXT("The measure tool is either not active or it has been destroyed."));
//    });
//    PlayerInputComponent->AddActionBinding(PlaceMarkerBinding);
//
//    FInputActionBinding InspectModeBinding( "InspectMode", IE_Released );
//    InspectModeBinding.ActionDelegate.GetDelegateForManualSet().BindLambda( [this]()
//    {
//        this->set_inspect_mode();
//    });
//    PlayerInputComponent->AddActionBinding(InspectModeBinding);
//
//    FInputActionBinding BallDropExperimentBinding( "BallDrop", IE_Released );
//    BallDropExperimentBinding.ActionDelegate.GetDelegateForManualSet().BindLambda( [this]()
//    {
//        this->set_ball_drop_experiment_mode();
//    });
//    PlayerInputComponent->AddActionBinding(BallDropExperimentBinding);
}

void ADuneAvatar::Tick(float delta_seconds)
{
//    DecoratorBase::update(delta_seconds);

//    camera_boom_->SetWorldTransform((mode_ != nullptr) ? mode_->calculate_camera_displacement(this->get_follow_camera()->GetForwardVector(), this->GetActorLocation()) : this->GetActorTransform());
//    camera_boom_->TargetArmLength = (mode_ != nullptr) ? mode_->calculate_camera_arm_length() : 300.0f;

//    detect_viable_interactions();
//    update_viable_interactions();
}

//void ADuneAvatar::OnResetVR()
//{
//	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
//}

void ADuneAvatar::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    
        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ADuneAvatar::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

//void ADuneAvatar::detect_viable_interactions()
//{
//    TArray<AActor*> collected_actors;
//    get_collection_sphere()->GetOverlappingActors(collected_actors, APalpableActor::StaticClass());
//
//    /// Iterate through all collected actors.
//    for (int32 i = 0; i < collected_actors.Num(); ++i)
//    {
//        /// Try cast as a pickup.
//        APickupActor* const pickup_candidate = Cast<APickupActor>(collected_actors[i]);
//
//        /// Executed on active pickups in the sphere of influence.
//        if (pickup_candidate && !pickup_candidate->IsPendingKill())
//        {
//            if (pickup_candidate->actor_interaction_viable(this)  && !viable_interactions_.Find(pickup_candidate->GetName()))
//            {
//                auto viable_interaction = NewObject<UViableInteraction>();
//                viable_interaction->initialize(this, pickup_candidate);
//                viable_interactions_.Add(pickup_candidate->GetName(), viable_interaction);
//            }
//        }
//
//        /// Try cast as an observable.
//        AObservableActor* const observable_candidate = Cast<AObservableActor>(collected_actors[i]);
//
//        //Executed on active pickups in the sphere of influence.
//        if (observable_candidate && !observable_candidate->IsPendingKill())
//        {
//            if (observable_candidate->actor_interaction_viable(this) && !viable_interactions_.Find(observable_candidate->GetName()))
//            {
//                auto viable_interaction = NewObject<UViableInteraction>();
//                viable_interaction->initialize(this, observable_candidate);
//                viable_interactions_.Add(observable_candidate->GetName(), viable_interaction);
//            }
//        }
//    }
//}

//void ADuneAvatar::update_viable_interactions()
//{
//    for (auto itr = viable_interactions_.CreateConstIterator(); itr; ++itr)
//    {
//        //if interactions are no longer viable remove them.
//        if (!itr.Value()->is_viable())
//            viable_interactions_.Remove(itr.Key());
//    }
//}
//
//void ADuneAvatar::try_interaction()
//{
//    UE_LOG(LogClass, Log, TEXT("%s is trying to initiate an interaction. "), *this->GetName());
//
//    /// Initiates all interactions.
//    for (auto itr = viable_interactions_.CreateConstIterator(); itr; ++itr)
//    {
//        /// Get the value of the interaction.
//        auto interaction = itr.Value();
//
//        if (interaction)
//        {
//            interaction->commit();
//        }
//        else
//        {
//            UE_LOG(LogClass, Error, TEXT("No viable interaction found for the object: %s"), *itr.Key());
//        }
//    }
//}
//
//TArray<UPickupModel*> ADuneAvatar::get_collectibles() const
//{
//    return collectibles_;
//}

//UUserWidget * ADuneAvatar::display_pickup(TSubclassOf<UAvatarMenu> menu_type)
//{
//    UE_LOG(LogClass, Log, TEXT("display pickup called"));
//
//    UUserWidget * widget = nullptr;
//    auto controller = this->GetController<ADuneController>();
//
//    //tear down any existing mode.
//    if (mode_ != nullptr)
//    {
//        mode_->teardown();
//    }
//
//    if (!menu_type)
//    {
//        UE_LOG(LogClass, Error, TEXT("Tool class was not found. "));
//        mode_ = nullptr;
//    }
//    else
//    {
//        UE_LOG(LogClass, Log, TEXT("Tool class was found. "));
//
//        //assign a new mode instance and set it up.
//        if (controller != nullptr)
//        {
//            auto menu = NewObject<UAvatarMenu>(this, menu_type);
//            mode_ = menu;
//            widget = controller->update_hud();
//
//            //mode_ was reassigned therefore check that it is still valid.
//            if (mode_ != nullptr)
//            {
//                UE_LOG(LogClass, Log, TEXT("Setup new mode."));
//                mode_->setup( this, &GetWorldTimerManager() );
//            }
//            else
//            {
//                UE_LOG(LogClass, Error, TEXT("Mode reference was not valid. "));
//            }
//        }
//        else
//        {
//            UE_LOG(LogClass, Error, TEXT("Controller reference was not valid. "));
//        }
//
//        return widget;
//    }
//
//    return nullptr;
//}

//TMap<FString, class UViableInteraction *> ADuneAvatar::get_viable_interactions() const
//{
//    return viable_interactions_;
//}
//
//bool ADuneAvatar::add_collectible(UPickupModel * collectible_data)
//{
//    if (collectible_data != nullptr)
//    {
//        UE_LOG(LogClass, Log, TEXT("Player collected: %s"), *collectible_data->get_name());
//        collectibles_.Add(collectible_data);
//        return true;
//    }
//    else
//    {
//        UE_LOG(LogClass, Warning, TEXT("Collectible returned null data."));
//        return false;
//    }
//}
//
//void ADuneAvatar::set_inspect_mode()
//{
//    auto tool = Cast<UAvatarTool>(mode_);
//
//    if (!tool || tool->get_tool_type() != EAvatarTool::InspectTool)
//    {
//        this->use_tool(EAvatarTool::InspectTool);
//    }
//    else
//    {
//        this->set_mode(EAvatarMode::Roam);
//    }
//
//}
//
//void ADuneAvatar::set_measure_mode()
//{
//    auto tool = Cast<UAvatarTool>(mode_);
//
//    if (!tool || tool->get_tool_type() != EAvatarTool::MeasureTool)
//    {
//        this->use_tool(EAvatarTool::MeasureTool);
//    }
//    else
//    {
//        this->set_mode(EAvatarMode::Roam);
//    }
//
//}

//void ADuneAvatar::set_ball_drop_experiment_mode()
//{
//    auto tool = Cast<UAvatarTool>(mode_);
//
//    if (!tool || tool->get_tool_type() != EAvatarTool::BallDropExperiment)
//    {
//        this->use_tool(EAvatarTool::BallDropExperiment);
//    }
//    else
//    {
//        this->set_mode(EAvatarMode::Roam);
//    }
//}
//
//void ADuneAvatar::use_tool(EAvatarTool tool)
//{
//    TSubclassOf<UAvatarMode>*  type = available_tool_.Find(tool);
//    this->set_user_mode(type);
//}
//
//void ADuneAvatar::set_mode(EAvatarMode mode)
//{
//    TSubclassOf<UAvatarMode>* type = available_mode_.Find(mode);
//    this->set_user_mode(type);
//}
