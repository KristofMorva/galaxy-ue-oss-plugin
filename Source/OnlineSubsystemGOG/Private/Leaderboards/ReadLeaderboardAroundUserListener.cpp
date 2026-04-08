#include "ReadLeaderboardAroundUserListener.h"

#include "OnlineLeaderboardsGOG.h"

FReadLeaderboardAroundUserListener::FReadLeaderboardAroundUserListener(
	class FOnlineLeaderboardsGOG& InLeaderboardsInterface,
	TSharedRef<const FUniqueNetIdGOG> InPlayer,
	uint32 InRange,
	FOnlineLeaderboardReadRef& InOutReadLeaderboard)
	: FLeaderboardRetriever{InLeaderboardsInterface, MoveTemp(InOutReadLeaderboard)}
	, player{InPlayer}
	, range{InRange}
{
}

void FReadLeaderboardAroundUserListener::RequestLeaderboardEntries()
{
	galaxy::api::Stats()->RequestLeaderboardEntriesAroundUser(TCHAR_TO_UTF8(*NameToString(readLeaderboard->LeaderboardName)), range, range, *player, this);

	auto err = galaxy::api::GetError();
	if (err)
	{
		UE_LOG_ONLINE_LEADERBOARD(Error, TEXT("Failed to request leaderboard entries around user: leaderboardName='%s', playerID='%s', range=%u; %s; %s"),
			*NameToString(readLeaderboard->LeaderboardName), *player->ToString(), range, UTF8_TO_TCHAR(err->GetName()), UTF8_TO_TCHAR(err->GetMsg()));

		TriggerOnLeaderboardReadCompleteDelegates(false);
		return;
	}
}
