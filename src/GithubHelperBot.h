#ifndef GITHUBHELPERBOT_GITHUBHELPERBOT_H
#define GITHUBHELPERBOT_GITHUBHELPERBOT_H

#include <string>
#include <tgbot/tgbot.h>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <pqxx/pqxx>
#include <unordered_map>

class GithubHelperBot {
private:
    std::string token;
    std::unordered_map<int64_t, std::string> user;
    std::unordered_map<int64_t, std::string> repositoryName;
    std::unordered_map<int64_t, int> pullRequestNumber;
    std::unordered_map<int64_t, int> issueNumber;
    std::unordered_map<int64_t, std::string> link;
    std::unordered_map<int64_t, std::string> description;
    std::string tableName = "ghb.\"favouriteRecords\"";
    std::unordered_map<int64_t, int> recordNumber;
public:
    explicit GithubHelperBot(const std::string &token);
private:


    TgBot::InlineKeyboardMarkup::Ptr setUpMainMenu();
    TgBot::InlineKeyboardMarkup::Ptr setUpStatisticsMenu();
    TgBot::InlineKeyboardMarkup::Ptr setUpSearchMenu();
    TgBot::InlineKeyboardMarkup::Ptr setUpFavouritesMenu();

    void setUpInputParameters(TgBot::Bot &bot);

    void onCommandStart(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryStatisticsMenu(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQuerySearchMenu(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryFavoritesMenu(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryExitToMainMenu(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);

    void onCallbackQueryRepositoryStatistics(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryUserStatistics(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryPullRequestStatistics(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryIssueStatistics(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);

//    void onCallbackQuerySearchRepositoryies(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);


    void onCallbackQueryAddNewRecordToFavourites(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryShowFavouriteRecords(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);


};


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

#endif //GITHUBHELPERBOT_GITHUBHELPERBOT_H
