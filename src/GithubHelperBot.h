#ifndef GITHUBHELPERBOT_GITHUBHELPERBOT_H
#define GITHUBHELPERBOT_GITHUBHELPERBOT_H

#include <string>
#include <unordered_map>
#include <tgbot/tgbot.h>

class GithubHelperBot {
private:
    std::string token;
public:
    explicit GithubHelperBot(const std::string &token);
private:

    TgBot::InlineKeyboardMarkup::Ptr setUpMainMenu();
    TgBot::InlineKeyboardMarkup::Ptr setUpStatisticsMenu();
    TgBot::InlineKeyboardMarkup::Ptr setUpSearchMenu();
    TgBot::InlineKeyboardMarkup::Ptr setUpFavouritesMenu();


//    void onCommandInfo(TgBot::Bot &bot, TgBot::Message::Ptr message);
//    void onCommandStart(TgBot::Bot &bot, TgBot::Message::Ptr message);
//    void onCommandKeyboard(TgBot::Bot &bot, TgBot::Message::Ptr message);

};

#endif //GITHUBHELPERBOT_GITHUBHELPERBOT_H
