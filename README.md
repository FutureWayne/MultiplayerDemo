# Steam Multiplayer Plugin
This is a plugin that utilizes Unreal Online Subsystem to manage multiplayer sessions on Steam, including a menu widget to host and join a session.

## Online Subsystem

### Unreal Online Subsystem
Access functionality of online services

### Session Interface
An instance of the game running on the server

### Steam Subsystem
Plugin: Online Subsystem Steam

Config: DefaultEngine.ini

Access to the Online Subsystem: 

`IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get()`

### Create Session
1.  Check and destroy existing session
```
auto ExistingSession = OnlineSessionInterface>GetNamedSession(NAME_GameSession); 
if (ExistingSession != nullptr)  
{  
    OnlineSessionInterface->DestroySession(NAME_GameSession);  
}
```

2.  Bind callback to on session created
```
FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate
OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
```

3. Construct session setting
```
TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
```

4. Create session through online session interface
```
const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
```
### Find Session

1. Construct session search
```
TSharedPtr<FOnlineSessionSearch> SessionSearch = MakeShareable(new FOnlineSessionSearch());
```

2. Bind callback to on session find
```
FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
```

3. Find session through online session interface
```
const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();  
OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
```

4. In find session callback, loop through search result
```
for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
```

### Join Session
1.  Bind callback of On Join Session and call join using the search result
```
FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();  
OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchResult);
```

2. In join session callback, use Client travel
```
FString Address;  
if (OnlineSessionInterface->GetResolvedConnectString(SessionName, Address))  
{  
    if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())  
    {       
	    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute); 
    }
}
```

## Create Customed Online Subsystem Plugin

### What parent class?
1. Game Instance?
	- Spawned at game creation
	- Not destroyed until the game is shut down
	- Persists between levels
2. Subsystems
	- Created after the game instance is created
	- Destroyed and garbage collected when the Game Instance is shut down

	[Programming Subsystems in Unreal Engine | Unreal Engine 5.3 Documentation | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/en-us/unreal-engine/programming-subsystems-in-unreal-engine?application_version=5.3)

## Menu Class

### Widget response to create session

1. The menu widget class can get the `MultiplayerSessionSubsystem` from the `GameInstance`:
```
if (const UGameInstance* GameInstance = GetGameInstance())  
{  
    MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();  
}
```

2. Bind button in widget with variables in C++:
```
UPROPERTY(meta = (BindWidget))  
TObjectPtr<UButton> HostButton;
```

3. Bind callbacks of buttons in the overridden `Initialize` function:
```
bool UMenu::Initialize()  
{  
    if (!Super::Initialize())  
    {       return false;  
    }  
    if (HostButton)  
    {       HostButton->OnClicked.AddDynamic(this, &UMenu::OnHostButtonClicked);  
    }
}
```

4. Manage destroying of the widget in the overridden `NativeDestruct` function

### Widget class register callbacks to subsystem functions

Ensures one-way dependency: Widget -> Subsystem -> Session Interface

1. Custom delegate in subsystem:
```
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
```

2. Bind callbacks in menu widget class
```
if (MultiplayerSessionSubsystem)  
{  
    MultiplayerSessionSubsystem->MultiplayerOnCreateSessionCompleteDelegate.AddDynamic(this, &UMenu::OnCreateSessionComplete);  
}
```




## Tracking Incoming Players

### Two important class in multiplayer

1. Game Mode
	- Rules for the game
	- Moving players to levels
	- Selecting spawn locations
	- Track when player joins and leaves the game
2. Game State
	- Let clients monitor the state of the game
	- Not specific to individual players
	- Array of player states




### Track Player Login & Logout

In game mode overridden functions, player is tracked when they join and exit the game:

```
virtual void PostLogin(APlayerController* NewPlayer) override;  
virtual void Logout(AController* Exiting) override;
```

