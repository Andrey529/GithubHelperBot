#include "GithubHelperBot.h"

GithubHelperBot::GithubHelperBot(const std::string &token) : token(token) {
    TgBot::Bot bot(token);

    TgBot::InlineKeyboardMarkup::Ptr mainKeyboard = setUpMainMenu();
    TgBot::InlineKeyboardMarkup::Ptr statisticsKeyboard = setUpStatisticsMenu();
    TgBot::InlineKeyboardMarkup::Ptr searchKeyboard = setUpSearchMenu();
    TgBot::InlineKeyboardMarkup::Ptr favouritesKeyboard = setUpFavouritesMenu();

    onCommandStart(bot, mainKeyboard);
    onCallbackQueryStatisticsMenu(bot, statisticsKeyboard);
    onCallbackQuerySearchMenu(bot, searchKeyboard);
    onCallbackQueryFavoritesMenu(bot, favouritesKeyboard);
    onCallbackQueryExitToMainMenu(bot, mainKeyboard);

    setUpInputParameters(bot);

    onCallbackQueryRepositoryStatistics(bot, mainKeyboard);
    onCallbackQueryUserStatistics(bot, mainKeyboard);


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

void GithubHelperBot::onCommandStart(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCommand("start", [&bot, &keyboard](TgBot::Message::Ptr message) {
        std::string text = "Привет, это GithubAwesomeBot.\n"
                           "Бот может:\n"
                           "1) Искать репозитории и пользователей\n"
                           "2) Выводить статистику по пользователю, репозиторию, issue и pull request\n"
                           "3) Сохранять в избранное понравившиеся тебе ссылки c интересными репозиториями, пользователями и другим.\n\n"
                           "Перед началом использования бота необходимо ввести некоторые параметры:\n"
                           "1) Для ввода имени владельца репозитория используйте шаблон: \"Владелец: <никнейм владельца репозитория>\".\n"
                           "2) Для ввода названия репозитория используйте шаблон: \"Репозиторий: <название репозитория>\".\n"
                           "3) Для ввода номера pull request'а используйте шаблон: \"Номер pull request: <номер pull request'a>\".\n"
                           "4) Для ввода номера issue используйте шаблон: \"Номер issue: <номер issue>\".\n";
        bot.getApi().sendMessage(message->chat->id, text, false, 0, keyboard);
    });
}

void GithubHelperBot::onCallbackQueryStatisticsMenu(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "statisticsMenu")) {
            std::string response = "Выберете нужную опцию для вывода статистики";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
    });
}

void GithubHelperBot::onCallbackQuerySearchMenu(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "searchMenu")) {
            std::string response = "Выберете нужную опцию для поиска";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
    });
}

void GithubHelperBot::onCallbackQueryFavoritesMenu(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "favoritesMenu")) {
            std::string response = "Выберете нужную опцию";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
    });
}

void GithubHelperBot::onCallbackQueryExitToMainMenu(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "exitStatistics") ||
            StringTools::startsWith(query->data, "exitSearch") ||
            StringTools::startsWith(query->data, "exitFavourites")
                ) {
            std::string response = "Возврат к главному меню";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
    });
}

void GithubHelperBot::setUpInputParameters(TgBot::Bot &bot) {
    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message){
        std::string tag = "Владелец: ";
        if (StringTools::startsWith(message->text, tag)) {
            std::cout << "User wrote: " << message->text << std::endl;
            user = message->text.erase(0, tag.size());
            std::cout << "user: " << user << std::endl;
            bot.getApi().sendMessage(message->chat->id, "Введен владелец: " + user);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message){
        std::string tag = "Репозиторий: ";
        if (StringTools::startsWith(message->text, tag)) {
            std::cout << "User wrote: " << message->text << std::endl;
            repositoryName = message->text.erase(0, tag.size());
            std::cout << "repositoryName: " << repositoryName << std::endl;
            bot.getApi().sendMessage(message->chat->id, "Введен репозиторий: " + repositoryName);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message){
        std::string tag = "Номер pull request: ";
        if (StringTools::startsWith(message->text, tag)) {
            std::cout << "User wrote: " << message->text << std::endl;
            std::string temp = message->text.erase(0, tag.size());
            pullRequestNumber = std::stoi(temp);
            std::cout << "pullRequestNumber: " << pullRequestNumber << std::endl;
            bot.getApi().sendMessage(message->chat->id, "Введен номер pull request'а: " + temp);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message){
        std::string tag = "Номер issue: ";
        if (StringTools::startsWith(message->text, tag)) {
            std::cout << "User wrote: " << message->text << std::endl;
            std::string temp = message->text.erase(0, tag.size());
            issueNumber = std::stoi(temp);
            std::cout << "issueNumber: " << issueNumber << std::endl;
            bot.getApi().sendMessage(message->chat->id, "Введен номер issue: " + temp);
        }
    });
}

void GithubHelperBot::onCallbackQueryRepositoryStatistics(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "repositoryStatistics")) {
            if (user.empty() || repositoryName.empty()) {
                std::string requestToEnterUserAndRepositoryOwner = "Введите владельца и название репозитория:";
                bot.getApi().sendMessage(query->message->chat->id, requestToEnterUserAndRepositoryOwner);
            }
            else {
                CURL *curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if (curl) {
                    std::string url = "https://api.github.com/repos/" + user + "/" + repositoryName;
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Accept: application/vnd.github+json");
                    curl_easy_setopt(curl, CURLOPT_USERAGENT,
                                     "Authorization: Bearer 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");
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, "X-GitHub-Api-Version: 2022-11-28");

                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                    res = curl_easy_perform(curl);
                    curl_easy_cleanup(curl);

                    if (res == 0) {
                        std::stringstream jsonEncoded(readBuffer);
                        boost::property_tree::ptree root;
                        boost::property_tree::read_json(jsonEncoded, root);

                        if (root.empty())
                            return;

                        std::string response = "Статистика репозитория с владельцем: " + user + " и названием репозитория: " +
                                               repositoryName + "\n" +
                                               "Repository url: " + root.get<std::string>("html_url") + "\n" +
                                               "Repository name: " + root.get<std::string>("name") + "\n" +
                                               "Visibility: " + root.get<std::string>("visibility") + "\n" +
                                               "Forks count: " + root.get<std::string>("forks") + "\n" +
                                               "Open issues count: " + root.get<std::string>("open_issues") + "\n" +
                                               "Stars count: " + root.get<std::string>("stargazers_count") + "\n" +
                                               "Watchers count: " + root.get<std::string>("watchers_count") + "\n" +
                                               "Forks count: " + root.get<std::string>("forks_count") + "\n" +
                                               "Main language: " + root.get<std::string>("language") + "\n";

                        std::cout << response << std::endl;
                        bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard);
                    }
                    else {
                        bot.getApi().sendMessage(query->message->chat->id, "Не удалось найти репозиторий с параметрами: "
                            "владелец = " + user + ", название репозитория = " + repositoryName,
                            false, 0, keyboard);
                    }
                }
            }
        }
    });
}

void GithubHelperBot::onCallbackQueryUserStatistics(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "userStatistics")) {
            if (user.empty()) {
                std::string requestToEnterUser = "Введите имя пользователя";
                bot.getApi().sendMessage(query->message->chat->id, requestToEnterUser);
            }
            else {
                CURL *curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if (curl) {
                    std::string url = "https://api.github.com/users/" + user;
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Accept: application/vnd.github+json");
                    curl_easy_setopt(curl, CURLOPT_USERAGENT,
                                     "Authorization: Bearer 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");
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, "X-GitHub-Api-Version: 2022-11-28");

                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                    res = curl_easy_perform(curl);
                    curl_easy_cleanup(curl);

                    if (res == 0) {
                        std::stringstream jsonEncoded(readBuffer);
                        boost::property_tree::ptree root;
                        boost::property_tree::read_json(jsonEncoded, root);

                        if (root.empty())
                            return;

                        std::string response = "Статистика пользователя: " + user + "\n" +
                                               "User url: " + root.get<std::string>("html_url") + "\n" +
                                               "Repository name: " + root.get<std::string>("name") + "\n" +
                                               "Name: " + root.get<std::string>("name") + "\n" +
                                               "Company: " + root.get<std::string>("company") + "\n" +
                                               "Location: " + root.get<std::string>("location") + "\n" +
                                               "Email: " + root.get<std::string>("email") + "\n" +
                                               "Public repos count: " + root.get<std::string>("public_repos") + "\n" +
                                               "Followers count: " + root.get<std::string>("followers") + "\n" +
                                               "Following count: " + root.get<std::string>("following") + "\n";

                        std::cout << response << std::endl;
                        bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard);
                    }
                    else {
                        bot.getApi().sendMessage(query->message->chat->id, "Не удалось найти пользователя: " + user, false, 0, keyboard);
                    }
                }
            }
        }
    });
}


