
## Connection type

### Peer to Peer

A lot of network traffic
No authority version

### Client Server

Authoritative 
Listen Server / Dedicated Server
Unreal Engine uses this

### LAN Connection
Listen Server:
`World->ServerTravel(“MapNameString ? listen”);`

Client:
`PlayerController->ClientTravel(“ipAddress”, ETravelType::TRAVEL_Absolute);`

## Online Subsystem

### Unreal Online Subsystem
Access functionality of online services

### Session interface
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
    {       PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute); 
    }}
```