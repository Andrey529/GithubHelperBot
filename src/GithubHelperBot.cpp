#include "GithubHelperBot.h"

GithubHelperBot::GithubHelperBot(const std::string &token) : token(token) {
    TgBot::Bot bot(token);

    TgBot::InlineKeyboardMarkup::Ptr mainKeyboard = setUpMainMenu();
    TgBot::InlineKeyboardMarkup::Ptr statisticsKeyboard = setUpStatisticsMenu();
    TgBot::InlineKeyboardMarkup::Ptr searchKeyboard = setUpSearchMenu();
    TgBot::InlineKeyboardMarkup::Ptr favouritesKeyboard = setUpFavouritesMenu();

    bot.getEvents().onCommand("start", [&bot, &mainKeyboard](TgBot::Message::Ptr message) {
        std::string text = "Привет, это GithubAwesomeBot.\n"
                           "Бот может:\n"
                           "1) Искать репозитории и пользователей\n"
                           "2) Выводить статистику по пользователю, репозиторию, issue и pull request\n"
                           "3) Сохранять в избранное понравившиеся тебе ссылки c интересными репозиториями, пользователями и другим.\n";
        bot.getApi().sendMessage(message->chat->id, text, false, 0, mainKeyboard);
    });

    bot.getEvents().onCallbackQuery([&bot, &statisticsKeyboard](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "statisticsMenu")) {
            std::string response = "Выберете нужную опцию для вывода статистики";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, statisticsKeyboard, "Markdown");
        }
    });

    bot.getEvents().onCallbackQuery([&bot, &searchKeyboard](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "searchMenu")) {
            std::string response = "Выберете нужную опцию для поиска";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, searchKeyboard, "Markdown");
        }
    });

    bot.getEvents().onCallbackQuery([&bot, &favouritesKeyboard](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "favoritesMenu")) {
            std::string response = "Выберете нужную опцию";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, favouritesKeyboard, "Markdown");
        }
    });



    bot.getEvents().onCallbackQuery([&bot, &mainKeyboard](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "exitStatistics") ||
            StringTools::startsWith(query->data, "exitSearch") ||
            StringTools::startsWith(query->data, "exitFavourites")
                ) {
            std::string response = "Возврат к главному меню";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, mainKeyboard, "Markdown");
        }
    });



    signal(SIGINT, [](int s) {
        std::cout << "SIGINT got" << std::endl;
        exit(0);
    });

    try {
        std::cout << "Bot username: %s\n" << bot.getApi().getMe()->username.c_str() << std::endl;
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            std::cout << "Long poll started" << std::endl;
            longPoll.start();
        }
    } catch (std::exception &e) {
        std::cout << "error: %s\n" << e.what() << std::endl;
    }

}

TgBot::InlineKeyboardMarkup::Ptr GithubHelperBot::setUpMainMenu() {
    TgBot::InlineKeyboardButton::Ptr searchButton(new TgBot::InlineKeyboardButton);
    searchButton->text = "Поиск";
    searchButton->callbackData = "searchMenu";
    TgBot::InlineKeyboardButton::Ptr statisticsButton(new TgBot::InlineKeyboardButton);
    statisticsButton->text = "Статистика";
    statisticsButton->callbackData = "statisticsMenu";
    TgBot::InlineKeyboardButton::Ptr favoritesButton(new TgBot::InlineKeyboardButton);
    favoritesButton->text = "Избранное";
    favoritesButton->callbackData = "favoritesMenu";

    std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
    row0.push_back(searchButton);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
    row1.push_back(statisticsButton);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row2;
    row2.push_back(favoritesButton);

    TgBot::InlineKeyboardMarkup::Ptr mainKeyboard(new TgBot::InlineKeyboardMarkup);
    mainKeyboard->inlineKeyboard.push_back(row0);
    mainKeyboard->inlineKeyboard.push_back(row1);
    mainKeyboard->inlineKeyboard.push_back(row2);

    return mainKeyboard;
}

TgBot::InlineKeyboardMarkup::Ptr GithubHelperBot::setUpStatisticsMenu() {
    TgBot::InlineKeyboardButton::Ptr repositoryStatisticsButton(new TgBot::InlineKeyboardButton);
    repositoryStatisticsButton->text = "Статистика репозитория";
    repositoryStatisticsButton->callbackData = "repositoryStatistics";

    TgBot::InlineKeyboardButton::Ptr userStatisticsButton(new TgBot::InlineKeyboardButton);
    userStatisticsButton->text = "Статистика пользователя";
    userStatisticsButton->callbackData = "userStatistics";

    TgBot::InlineKeyboardButton::Ptr pullRequestStatisticsButton(new TgBot::InlineKeyboardButton);
    pullRequestStatisticsButton->text = "Статистика pull request";
    pullRequestStatisticsButton->callbackData = "pullRequestSStatistics";

    TgBot::InlineKeyboardButton::Ptr issueStatisticsButton(new TgBot::InlineKeyboardButton);
    issueStatisticsButton->text = "Статистика issue";
    issueStatisticsButton->callbackData = "issueStatistics";

    TgBot::InlineKeyboardButton::Ptr exitStatisticsButton(new TgBot::InlineKeyboardButton);
    exitStatisticsButton->text = "Отмена";
    exitStatisticsButton->callbackData = "exitStatistics";

    TgBot::InlineKeyboardButton::Ptr addToFavouritesStatisticsButton(new TgBot::InlineKeyboardButton);
    addToFavouritesStatisticsButton->text = "Добавить в избранное";
    addToFavouritesStatisticsButton->callbackData = "addToFavouritesStatistics";

    std::vector<TgBot::InlineKeyboardButton::Ptr> statisticsRow0;
    statisticsRow0.push_back(repositoryStatisticsButton);
    statisticsRow0.push_back(userStatisticsButton);
    std::vector<TgBot::InlineKeyboardButton::Ptr> statisticsRow1;
    statisticsRow1.push_back(pullRequestStatisticsButton);
    statisticsRow1.push_back(issueStatisticsButton);
    std::vector<TgBot::InlineKeyboardButton::Ptr> statisticsRow2;
    statisticsRow2.push_back(addToFavouritesStatisticsButton);
    statisticsRow2.push_back(exitStatisticsButton);

    TgBot::InlineKeyboardMarkup::Ptr statisticsKeyboard(new TgBot::InlineKeyboardMarkup);
    statisticsKeyboard->inlineKeyboard.push_back(statisticsRow0);
    statisticsKeyboard->inlineKeyboard.push_back(statisticsRow1);
    statisticsKeyboard->inlineKeyboard.push_back(statisticsRow2);

    return statisticsKeyboard;
}

TgBot::InlineKeyboardMarkup::Ptr GithubHelperBot::setUpSearchMenu() {
    TgBot::InlineKeyboardButton::Ptr searchUserButton(new TgBot::InlineKeyboardButton);
    searchUserButton->text = "Поиск пользователя";
    searchUserButton->callbackData = "searchUser";

    TgBot::InlineKeyboardButton::Ptr searchRepositoryButton(new TgBot::InlineKeyboardButton);
    searchRepositoryButton->text = "Поиск репозитория";
    searchRepositoryButton->callbackData = "searchRepository";

    TgBot::InlineKeyboardButton::Ptr exitSearchButton(new TgBot::InlineKeyboardButton);
    exitSearchButton->text = "Отмена";
    exitSearchButton->callbackData = "exitSearch";

    TgBot::InlineKeyboardButton::Ptr addToFavouritesSearchButton(new TgBot::InlineKeyboardButton);
    addToFavouritesSearchButton->text = "Добавить в избранное";
    addToFavouritesSearchButton->callbackData = "addToFavouritesSearch";

    std::vector<TgBot::InlineKeyboardButton::Ptr> searchRow0;
    searchRow0.push_back(searchUserButton);
    searchRow0.push_back(searchRepositoryButton);
    std::vector<TgBot::InlineKeyboardButton::Ptr> searchRow1;
    searchRow1.push_back(addToFavouritesSearchButton);
    searchRow1.push_back(exitSearchButton);

    TgBot::InlineKeyboardMarkup::Ptr searchKeyboard(new TgBot::InlineKeyboardMarkup);
    searchKeyboard->inlineKeyboard.push_back(searchRow0);
    searchKeyboard->inlineKeyboard.push_back(searchRow1);

    return searchKeyboard;
}

TgBot::InlineKeyboardMarkup::Ptr GithubHelperBot::setUpFavouritesMenu() {
    TgBot::InlineKeyboardButton::Ptr addNewLinkButton(new TgBot::InlineKeyboardButton);
    addNewLinkButton->text = "Добавить новую ссылку";
    addNewLinkButton->callbackData = "addNewLink";

    TgBot::InlineKeyboardButton::Ptr deleteLinkButton(new TgBot::InlineKeyboardButton);
    deleteLinkButton->text = "Удалить ссылку";
    deleteLinkButton->callbackData = "deleteLink";

    TgBot::InlineKeyboardButton::Ptr editLinkButton(new TgBot::InlineKeyboardButton);
    editLinkButton->text = "Редактировать ссылку";
    editLinkButton->callbackData = "editLink";

    TgBot::InlineKeyboardButton::Ptr exitFavouritesButton(new TgBot::InlineKeyboardButton);
    exitFavouritesButton->text = "Отмена";
    exitFavouritesButton->callbackData = "exitFavourites";

    TgBot::InlineKeyboardButton::Ptr showFavouritesLinkButton(new TgBot::InlineKeyboardButton);
    showFavouritesLinkButton->text = "Показать список сохраненных ссылок";
    showFavouritesLinkButton->callbackData = "showFavouritesLink";


    std::vector<TgBot::InlineKeyboardButton::Ptr> favouritesRow0;
    favouritesRow0.push_back(addNewLinkButton);
    favouritesRow0.push_back(deleteLinkButton);
    std::vector<TgBot::InlineKeyboardButton::Ptr> favouritesRow1;
    favouritesRow1.push_back(editLinkButton);
    favouritesRow1.push_back(exitFavouritesButton);
    std::vector<TgBot::InlineKeyboardButton::Ptr> favouritesRow2;
    favouritesRow2.push_back(showFavouritesLinkButton);

    TgBot::InlineKeyboardMarkup::Ptr favouritesKeyboard(new TgBot::InlineKeyboardMarkup);
    favouritesKeyboard->inlineKeyboard.push_back(favouritesRow0);
    favouritesKeyboard->inlineKeyboard.push_back(favouritesRow1);
    favouritesKeyboard->inlineKeyboard.push_back(favouritesRow2);

    return favouritesKeyboard;
}