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
    std::unordered_map<int64_t, std::string> link; // links for favourite records
    std::unordered_map<int64_t, std::string> description; // description for favourite links
    std::unordered_map<int64_t, std::string> content; // words that are used to search repository
    std::unordered_map<int64_t, std::string> languageInRepo;
    std::unordered_map<int64_t, std::string> repoVisibibillity; // public or private
    std::unordered_map<int64_t, int> minimumTopicsCountInRepo;
    std::unordered_map<int64_t, int> mininmumStarsCount;
    std::unordered_map<int64_t, int> minimumForksCount;
    std::string tableName = "ghb.\"favouriteRecords\"";
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

    void onCallbackQuerySearchRepositoryies(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQuerySearchDropAllParameters(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);


    void onCallbackQueryAddNewRecordToFavourites(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryShowFavouriteRecords(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryDeleteRecordFromFavourites(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);
    void onCallbackQueryEditFavouriteRecord(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard);

    std::string urlEncode(const std::string &value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (char c : value) {
            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
            escaped << std::nouppercase;
        }

        return escaped.str();
    }
};


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

#endif //GITHUBHELPERBOT_GITHUBHELPERBOT_H
