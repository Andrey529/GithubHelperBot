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
    onCallbackQueryPullRequestStatistics(bot, mainKeyboard);
    onCallbackQueryIssueStatistics(bot, mainKeyboard);

    onCallbackQueryAddNewRecordToFavourites(bot, mainKeyboard);
    onCallbackQueryShowFavouriteRecords(bot, mainKeyboard);
    onCallbackQueryDeleteRecordFromFavourites(bot, mainKeyboard);
    onCallbackQueryEditFavouriteRecord(bot, mainKeyboard);

    onCallbackQuerySearchRepositoryies(bot, mainKeyboard);
    onCallbackQuerySearchDropAllParameters(bot, mainKeyboard);

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
    TgBot::InlineKeyboardButton::Ptr dropAllParametersButton(new TgBot::InlineKeyboardButton);
    dropAllParametersButton->text = "Сбросить все параметры поиска";
    dropAllParametersButton->callbackData = "dropAllParameters";

    TgBot::InlineKeyboardButton::Ptr searchRepositoryButton(new TgBot::InlineKeyboardButton);
    searchRepositoryButton->text = "Поиск репозиториев";
    searchRepositoryButton->callbackData = "searchRepository";

    TgBot::InlineKeyboardButton::Ptr exitSearchButton(new TgBot::InlineKeyboardButton);
    exitSearchButton->text = "Отмена";
    exitSearchButton->callbackData = "exitSearch";

    TgBot::InlineKeyboardButton::Ptr addToFavouritesSearchButton(new TgBot::InlineKeyboardButton);
    addToFavouritesSearchButton->text = "Добавить в избранное";
    addToFavouritesSearchButton->callbackData = "addToFavouritesSearch";

    std::vector<TgBot::InlineKeyboardButton::Ptr> searchRow0;
    searchRow0.push_back(dropAllParametersButton);
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
    addNewLinkButton->text = "Добавить новую запись";
    addNewLinkButton->callbackData = "addNewRecord";

    TgBot::InlineKeyboardButton::Ptr deleteLinkButton(new TgBot::InlineKeyboardButton);
    deleteLinkButton->text = "Удалить запись";
    deleteLinkButton->callbackData = "deleteRecord";

    TgBot::InlineKeyboardButton::Ptr editLinkButton(new TgBot::InlineKeyboardButton);
    editLinkButton->text = "Редактировать запись";
    editLinkButton->callbackData = "editRecord";

    TgBot::InlineKeyboardButton::Ptr exitFavouritesButton(new TgBot::InlineKeyboardButton);
    exitFavouritesButton->text = "Отмена";
    exitFavouritesButton->callbackData = "exitFavourites";

    TgBot::InlineKeyboardButton::Ptr showFavouritesLinkButton(new TgBot::InlineKeyboardButton);
    showFavouritesLinkButton->text = "Показать список сохраненных записей";
    showFavouritesLinkButton->callbackData = "showFavouritesRecord";


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
                           "1) Искать репозитории\n"
                           "2) Выводить статистику по пользователю, репозиторию, issue и pull request\n"
                           "3) Сохранять в избранное понравившиеся тебе ссылки c интересными репозиториями, пользователями и другим.\n\n"
                           "Перед началом использования бота необходимо ввести некоторые параметры:\n"
                           "1) Для ввода имени владельца репозитория используйте шаблон: \"Владелец: <никнейм владельца репозитория>\".\n"
                           "2) Для ввода названия репозитория используйте шаблон: \"Репозиторий: <название репозитория>\".\n"
                           "3) Для ввода номера pull request'а используйте шаблон: \"Номер pull request: <номер pull request'a>\".\n"
                           "4) Для ввода номера issue используйте шаблон: \"Номер issue: <номер issue>\".\n"
                           "5) Для ввода ссылки используйте шаблон: \"Ссылка: <текст ссылки>\".\n"
                           "6) Для ввода описания используйте шаблон: \"Описание: <текст описания>\".\n"
                           "7) Для ввода ключевых слов для поиска используйте шаблон: \"Ключевое слово: <ключевое слово>\".\n"
                           "8) Для ввода основного языка используйте шаблон: \"Язык: <название языка>\".\n"
                           "9) Для ввода видимости репозитория используйте шаблон: \"Описание: <public/private>\".\n"
                           "10) Для ввода минимального количества топиков в репозитории используйте шаблон: \"Количество топиков: <число>\".\n"
                           "11) Для ввода минимального количества звезд в репозитории используйте шаблон: \"Количество звезд: <число>\".\n"
                           "12) Для ввода минимального количества форков в репозитории используйте шаблон: \"Количество форков: <число>\".\n";
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
        if (StringTools::startsWith(query->data, "favoritesMenu") ||
            StringTools::startsWith(query->data, "addToFavouritesSearch") ||
            StringTools::startsWith(query->data, "addToFavouritesStatistics")) {
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
    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Владелец: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (user.contains(id)) {
                user[id] = message->text.erase(0, tag.size());
            } else {
                user.insert(std::pair<int64_t, std::string>(id, message->text.erase(0, tag.size())));
            }
            std::cout << "user: " << user[id] << std::endl;
            bot.getApi().sendMessage(id, "Введен владелец: " + user[id]);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Репозиторий: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (repositoryName.contains(id)) {
                repositoryName[id] = message->text.erase(0, tag.size());
            } else {
                repositoryName.insert(std::pair<int64_t, std::string>(id, message->text.erase(0, tag.size())));
            }
            std::cout << "repositoryName: " << repositoryName[id] << std::endl;
            bot.getApi().sendMessage(id, "Введен репозиторий: " + repositoryName[id]);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Номер pull request: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (pullRequestNumber.contains(id)) {
                pullRequestNumber[id] = std::stoi(message->text.erase(0, tag.size()));
            } else {
                pullRequestNumber.insert(std::pair<int64_t, int>(id, std::stoi(message->text.erase(0, tag.size()))));
            }
            std::cout << "pullRequestNumber: " << pullRequestNumber[id] << std::endl;
            bot.getApi().sendMessage(id, "Введен номер pull request'а: " + std::to_string(pullRequestNumber[id]));
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Номер issue: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (issueNumber.contains(id)) {
                issueNumber[id] = std::stoi(message->text.erase(0, tag.size()));
            } else {
                issueNumber.insert(std::pair<int64_t, int>(id, std::stoi(message->text.erase(0, tag.size()))));
            }
            std::cout << "issueNumber: " << issueNumber[id] << std::endl;
            bot.getApi().sendMessage(id, "Введен номер issue: " + std::to_string(issueNumber[id]));
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Ссылка: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (link.contains(id)) {
                link[id] = message->text.erase(0, tag.size());
            } else {
                link.insert(std::pair<int64_t, std::string>(id, message->text.erase(0, tag.size())));
            }
            std::cout << "link: " << link[id] << std::endl;
            bot.getApi().sendMessage(id, "Введена ссылка: " + link[id]);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Описание: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (description.contains(id)) {
                description[id] = message->text.erase(0, tag.size());
            } else {
                description.insert(std::pair<int64_t, std::string>(id, message->text.erase(0, tag.size())));
            }
            std::cout << "description: " << description[id] << std::endl;
            bot.getApi().sendMessage(id, "Введено описание: " + description[id]);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Ключевое слово: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (content.contains(id)) {
                content[id] = "\"" + message->text.erase(0, tag.size()) + "\"";
            } else {
                content.insert(std::pair<int64_t, std::string>(id, "\"" + message->text.erase(0, tag.size()) + "\""));
            }
            std::cout << "content: " << content[id] << std::endl;
            bot.getApi().sendMessage(id, "Введено ключевое слово: " + content[id]);
            content[id] = urlEncode(content[id]);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Язык: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (languageInRepo.contains(id)) {
                languageInRepo[id] = message->text.erase(0, tag.size());
            } else {
                languageInRepo.insert(std::pair<int64_t, std::string>(id, message->text.erase(0, tag.size())));
            }
            std::cout << "languageInRepo: " << languageInRepo[id] << std::endl;
            bot.getApi().sendMessage(id, "Введен основной язык: " + languageInRepo[id]);
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Видимость: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            std::string tmp = message->text.erase(0, tag.size());
            if (tmp == "private" || tmp == "public") {
                if (repoVisibibillity.contains(id)) {
                    repoVisibibillity[id] = tmp;
                } else {
                    repoVisibibillity.insert(std::pair<int64_t, std::string>(id, tmp));
                }
                std::cout << "repoVisibibillity: " << repoVisibibillity[id] << std::endl;
                bot.getApi().sendMessage(id, "Введена видимость репозитория: " + repoVisibibillity[id]);
            }
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Количество топиков: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (minimumTopicsCountInRepo.contains(id)) {
                minimumTopicsCountInRepo[id] = std::stoi(message->text.erase(0, tag.size()));
            } else {
                minimumTopicsCountInRepo.insert(
                        std::pair<int64_t, int>(id, std::stoi(message->text.erase(0, tag.size()))));
            }
            std::cout << "minimumTopicsCountInRepo: " << minimumTopicsCountInRepo[id] << std::endl;
            bot.getApi().sendMessage(id, "Введено минимальное количество топиков в репозитории: " +
                                         std::to_string(minimumTopicsCountInRepo[id]));
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Количество звезд: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (mininmumStarsCount.contains(id)) {
                mininmumStarsCount[id] = std::stoi(message->text.erase(0, tag.size()));
            } else {
                mininmumStarsCount.insert(std::pair<int64_t, int>(id, std::stoi(message->text.erase(0, tag.size()))));
            }
            std::cout << "mininmumStarsCount: " << mininmumStarsCount[id] << std::endl;
            bot.getApi().sendMessage(id, "Введено минимальное количество звезд в репозитории: " +
                                         std::to_string(mininmumStarsCount[id]));
        }
    });

    bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
        std::string tag = "Количество форков: ";
        if (StringTools::startsWith(message->text, tag)) {
            int64_t id = message->chat->id;
            std::cout << "User wrote: " << message->text << std::endl;
            if (minimumForksCount.contains(id)) {
                minimumForksCount[id] = std::stoi(message->text.erase(0, tag.size()));
            } else {
                minimumForksCount.insert(std::pair<int64_t, int>(id, std::stoi(message->text.erase(0, tag.size()))));
            }
            std::cout << "minimumForksCount: " << minimumForksCount[id] << std::endl;
            bot.getApi().sendMessage(id, "Введено минимальное количество форков в репозитории: " +
                                         std::to_string(minimumForksCount[id]));
        }
    });
}

void GithubHelperBot::onCallbackQueryRepositoryStatistics(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "repositoryStatistics")) {
            int64_t id = query->message->chat->id;
            if (user[id].empty() || !user.contains(id) || repositoryName[id].empty() || !repositoryName.contains(id)) {
                std::string requestToEnterUserAndRepositoryOwner = "Введите владельца и название репозитория:";
                bot.getApi().sendMessage(id, requestToEnterUserAndRepositoryOwner);
            } else {
                CURL *curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if (curl) {
                    std::string url = "https://api.github.com/repos/" + user[id] + "/" + repositoryName[id];
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

                        std::string response =
                                "Статистика репозитория с владельцем: " + user[id] + " и названием репозитория: " +
                                repositoryName[id] + "\n" +
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
                        bot.getApi().sendMessage(id, response, false, 0, keyboard);
                    } else {
                        bot.getApi().sendMessage(id, "Не удалось найти репозиторий с параметрами: "
                                                     "владелец = " + user[id] + ", название репозитория = " +
                                                     repositoryName[id],
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
            int64_t id = query->message->chat->id;
            if (user[id].empty() || !user.contains(id)) {
                std::string requestToEnterUser = "Введите имя пользователя";
                bot.getApi().sendMessage(id, requestToEnterUser);
            } else {
                CURL *curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if (curl) {
                    std::string url = "https://api.github.com/users/" + user[id];
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

                        std::string response = "Статистика пользователя: " + user[id] + "\n" +
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
                        bot.getApi().sendMessage(id, response, false, 0, keyboard);
                    } else {
                        bot.getApi().sendMessage(id, "Не удалось найти пользователя: " + user[id], false, 0, keyboard);
                    }
                }
            }
        }
    });
}

void GithubHelperBot::onCallbackQueryPullRequestStatistics(TgBot::Bot &bot,
                                                           TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "pullRequestSStatistics")) {
            int64_t id = query->message->chat->id;
            if (user[id].empty() || !user.contains(id) || repositoryName[id].empty()
                || !repositoryName.contains(id) || !pullRequestNumber.contains(id)) {
                std::string requestToEnterUserAndRepositoryOwnerAndPullRequestNumner =
                        "Введите владельца, название репозитория и номер pull request'а:";
                bot.getApi().sendMessage(id,
                                         requestToEnterUserAndRepositoryOwnerAndPullRequestNumner);
            } else {
                CURL *curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if (curl) {
                    std::string url =
                            "https://api.github.com/repos/" + user[id] + "/" + repositoryName[id] + "/pulls/" +
                            std::to_string(pullRequestNumber[id]);
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

                        std::string response =
                                "Статистика pull request'а с владельцем: " + user[id] + ", названием репозитория: " +
                                repositoryName[id] + " и номером:  " + std::to_string(pullRequestNumber[id]) + "\n" +
                                "Pull request url: " + root.get<std::string>("html_url") + "\n" +
                                "Issue url: " + root.get<std::string>("issue_url") + "\n" +
                                "Number: " + root.get<std::string>("number") + "\n" +
                                "State: " + root.get<std::string>("state") + "\n" +
                                "Title: " + root.get<std::string>("title") + "\n" +
                                "Opened by: " + root.get_child("user").get<std::string>("login") + "\n" +
                                "Body: " + root.get<std::string>("body") + "\n" +
                                "Created at: " + root.get<std::string>("created_at") + "\n" +
                                "Updated at: " + root.get<std::string>("updated_at") + "\n" +
                                "Closed at: " + root.get<std::string>("closed_at") + "\n" +
                                "Merged at: " + root.get<std::string>("merged_at") + "\n" +
                                "Assignee: " + root.get_child("assignee").get<std::string>("login") + "\n" +
                                "Requested reviewers: " + root.get<std::string>("requested_reviewers") + "\n" +
                                "Merged by: " + root.get_child("merged_by").get<std::string>("login") + "\n" +
                                "Comments: " + root.get<std::string>("comments") + "\n" +
                                "Review comments: " + root.get<std::string>("review_comments") + "\n" +
                                "Commits: " + root.get<std::string>("commits") + "\n";

                        std::cout << response << std::endl;
                        bot.getApi().sendMessage(id, response, false, 0, keyboard);
                    } else {
                        bot.getApi().sendMessage(id, "Не удалось найти pull request с параметрами: "
                                                     "пользователь = " + user[id] + ", название репозитория = " +
                                                     repositoryName[id] + ", номер pull request'а = " +
                                                     std::to_string(pullRequestNumber[id]),
                                                 false, 0, keyboard);
                    }
                }
            }
        }
    });

}

void GithubHelperBot::onCallbackQueryIssueStatistics(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "issueStatistics")) {
            int64_t id = query->message->chat->id;
            if (user[id].empty() || !user.contains(id) || repositoryName[id].empty()
                || !repositoryName.contains(id) || !issueNumber.contains(id)) {
                std::string requestToEnterUserAndRepositoryOwnerAndPullIssueNumner =
                        "Введите владельца, название репозитория и номер issue:";
                bot.getApi().sendMessage(id, requestToEnterUserAndRepositoryOwnerAndPullIssueNumner);
            } else {
                CURL *curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if (curl) {
                    std::string url = "https://api.github.com/repos/" + user[id] + "/" + repositoryName[id] +
                                      "/issues/" + std::to_string(issueNumber[id]);
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

                        std::string response =
                                "Статистика issue с владельцем: " + user[id] + ", названием репозитория: " +
                                repositoryName[id] + " и номером:  " + std::to_string(issueNumber[id]) + "\n" +
                                "Issue url: " + root.get<std::string>("html_url") + "\n" +
                                "Number: " + root.get<std::string>("number") + "\n" +
                                "Title: " + root.get<std::string>("title") + "\n" +
                                "State: " + root.get<std::string>("state") + "\n" +
                                "Opened by: " + root.get_child("user").get<std::string>("login") + "\n" +
                                "Body: " + root.get<std::string>("body") + "\n" +
                                "Assignee: " + root.get_child("assignee").get<std::string>("login") + "\n" +
                                "Comments count: " + root.get<std::string>("comments") + "\n" +
                                "Created at: " + root.get<std::string>("created_at") + "\n" +
                                "Updated at: " + root.get<std::string>("updated_at") + "\n" +
                                "Closed at: " + root.get<std::string>("closed_at") + "\n" +
                                "Closed by: " + root.get_child("closed_by").get<std::string>("login") + "\n";

                        std::cout << response << std::endl;
                        bot.getApi().sendMessage(id, response, false, 0, keyboard);
                    } else {
                        bot.getApi().sendMessage(id, "Не удалось найти issue с параметрами: "
                                                     "пользователь = " + user[id] + ", название репозитория = " +
                                                     repositoryName[id] + ", номер issue = " +
                                                     std::to_string(issueNumber[id]),
                                                 false, 0, keyboard);
                    }
                }
            }
        }
    });
}

void GithubHelperBot::onCallbackQueryAddNewRecordToFavourites(TgBot::Bot &bot,
                                                              TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "addNewRecord")) {
            int64_t id = query->message->chat->id;
            try {
                pqxx::connection connection("dbname = GithubHelperBot \
                            user = andrey \
                            password = 12345 \
                            hostaddr = 127.0.0.1 \
                            port = 5433");
                if (connection.is_open()) {
                    std::cout << "Opened database successfully: " << connection.dbname() << std::endl;
                } else {
                    std::cout << "Can't open database" << std::endl;
                    throw std::exception();
                }

                if (!link.contains(id)) {
                    bot.getApi().sendMessage(id, "Введите ссылку и описание ссылки для вставки новой записи", false, 0,
                                             keyboard);
                    connection.close();
                    return;
                }

                if (!description.contains(id)) {
                    description.insert(std::pair<int64_t, std::string>(id, "NULL"));
                } else if ((description.contains(id) && description[id].empty())) {
                    description[id] = "NULL";
                }

                std::string sql = "INSERT INTO " + tableName + " (\"telegramUserID\", link, \"description\")" \
                                  "VALUES (" + std::to_string(id) + ", \'" + link[id] + "\', \'" + description[id] +
                                  "\');";

                pqxx::work transaction(connection);
                transaction.exec(sql);
                transaction.commit();
                connection.close();

                std::cout << "Records added successfully" << std::endl;
                bot.getApi().sendMessage(id, "Успешно добавлена запись с ссылкой: "
                                             + link[id] + " и описанием: " + description[id], false, 0, keyboard);

            }
            catch (const std::exception &exception) {
                std::cerr << exception.what() << "\nПроизошла ошибка при вставке новой любимой записи" << std::endl;
                bot.getApi().sendMessage(id, "Произошла ошибка при вставке новой любимой записи", false, 0, keyboard);
            }
        }
    });
}

void GithubHelperBot::onCallbackQueryShowFavouriteRecords(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "showFavouritesRecord")) {
            try {
                pqxx::connection connection("dbname = GithubHelperBot \
                            user = andrey \
                            password = 12345 \
                            hostaddr = 127.0.0.1 \
                            port = 5433");
                if (connection.is_open()) {
                    std::cout << "Opened database successfully: " << connection.dbname() << std::endl;
                } else {
                    std::cout << "Can't open database" << std::endl;
                    throw std::exception();
                }

                std::string sql = "SELECT link, \"description\" "
                                  "FROM " + tableName + ""
                                                        "WHERE \"telegramUserID\" = " +
                                  std::to_string(query->message->chat->id) + ";";

                pqxx::work transaction(connection);
                pqxx::result result = transaction.exec(sql);
                transaction.commit();
                connection.close();

                if (result.empty()) {
                    std::cout << "All records showed successfully" << std::endl;
                    bot.getApi().sendMessage(query->message->chat->id, "У вас еще нет записей", false, 0, keyboard);
                    return;
                }

                int count = 1;
                std::string resultString;
                for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c) {
                    resultString += std::to_string(count) + ") ссылка: " + c[0].as<std::string>()
                                    + ", описание: " + c[1].as<std::string>() + "\n";
                    count++;
                }

                std::cout << "All records showed successfully" << std::endl;
                bot.getApi().sendMessage(query->message->chat->id, resultString, false, 0, keyboard);

            }
            catch (const std::exception &exception) {
                std::cerr << exception.what() << "\nПроизошла ошибка при выводе списка всех записей" << std::endl;
                bot.getApi().sendMessage(query->message->chat->id, "Произошла ошибка при выводе списка всех записей",
                                         false, 0, keyboard);
            }
        }
    });
}

void GithubHelperBot::onCallbackQueryDeleteRecordFromFavourites(TgBot::Bot &bot,
                                                                TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "deleteRecord")) {
            int64_t id = query->message->chat->id;
            try {
                pqxx::connection connection("dbname = GithubHelperBot \
                            user = andrey \
                            password = 12345 \
                            hostaddr = 127.0.0.1 \
                            port = 5433");
                if (connection.is_open()) {
                    std::cout << "Opened database successfully: " << connection.dbname() << std::endl;
                } else {
                    std::cout << "Can't open database" << std::endl;
                    throw std::exception();
                }

                if (!link.contains(id)) {
                    bot.getApi().sendMessage(id, "Введите ссылку для удаления записи", false, 0, keyboard);
                    connection.close();
                    return;
                }

                std::string sql = "DELETE FROM " + tableName + " " \
                                  "WHERE link = \'" + link[id] + "\' AND \"telegramUserID\" = " + std::to_string(id) +
                                  ";";

                pqxx::work transaction(connection);
                transaction.exec(sql);
                transaction.commit();
                connection.close();

                std::cout << "Records deleted successfully" << std::endl;
                bot.getApi().sendMessage(id, "Успешно удалена запись с ссылкой: " + link[id] +
                                             " и описанием: " + description[id], false, 0, keyboard);

            }
            catch (const std::exception &exception) {
                std::cerr << exception.what() << "\nПроизошла ошибка при удалении записи" << std::endl;
                bot.getApi().sendMessage(id, "Произошла ошибка при удалении записи", false, 0, keyboard);
            }
        }
    });
}

void GithubHelperBot::onCallbackQueryEditFavouriteRecord(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "editRecord")) {
            int64_t id = query->message->chat->id;
            try {
                pqxx::connection connection("dbname = GithubHelperBot \
                            user = andrey \
                            password = 12345 \
                            hostaddr = 127.0.0.1 \
                            port = 5433");
                if (connection.is_open()) {
                    std::cout << "Opened database successfully: " << connection.dbname() << std::endl;
                } else {
                    std::cout << "Can't open database" << std::endl;
                    throw std::exception();
                }

                if (!link.contains(id)) {
                    bot.getApi().sendMessage(id, "Введите ссылку для изменения записи", false, 0, keyboard);
                    connection.close();
                    return;
                }

                std::string sql = "Update " + tableName + "" \
                                  "SET \"description\" = \'" + description[id] + "\'" \
                                  "WHERE link = \'" + link[id] + "\' AND \"telegramUserID\" = " + std::to_string(id) +
                                  ";";

                pqxx::work transaction(connection);
                transaction.exec(sql);
                transaction.commit();
                connection.close();

                std::cout << "Records updated successfully" << std::endl;
                bot.getApi().sendMessage(id, "Успешно обновлена запись с ссылкой: " + link[id] +
                                             " и описанием: " + description[id], false, 0, keyboard);

            }
            catch (const std::exception &exception) {
                std::cerr << exception.what() << "\nПроизошла ошибка при удалении записи" << std::endl;
                bot.getApi().sendMessage(id, "Произошла ошибка при удалении записи", false, 0, keyboard);
            }
        }
    });
}

void GithubHelperBot::onCallbackQuerySearchRepositoryies(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "searchRepository")) {
            int64_t id = query->message->chat->id;
            if (!user.contains(id) && !content.contains(id) && !languageInRepo.contains(id) && !repoVisibibillity.contains(id)
                && !minimumTopicsCountInRepo.contains(id) && !mininmumStarsCount.contains(id)
                && !minimumForksCount.contains(id)) {
                std::string requestToEnterUserAndRepositoryOwnerAndPullIssueNumner =
                        "Введите имя пользователя или ключевые слова или основной язык или видимость репозитория или "
                        "минимальное количество топиков в репозитории или минимальное количество звезд в репозитории"
                        "или минимальное количество форков репозитория для поиска нужных репозиториев:";
                bot.getApi().sendMessage(id, requestToEnterUserAndRepositoryOwnerAndPullIssueNumner);
            } else {
                CURL *curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if (curl) {
                    std::string url = "https://api.github.com/search/repositories?q=";
                    if (content.contains(id)) {
                        url += content[id];
                    }
                    if (user.contains(id)) {
                        url += "+user:" + user[id];
                    }
                    if (repoVisibibillity.contains(id)) {
                        if (repoVisibibillity[id] == "public" || repoVisibibillity[id] == "private") {
                            url += "+is:" + repoVisibibillity[id];
                        }
                    }
                    if (minimumTopicsCountInRepo.contains(id)) {
                        url += "+topics:" + std::to_string(minimumTopicsCountInRepo[id]);
                    }
                    if (mininmumStarsCount.contains(id)) {
                        url += "+stars:>=" + std::to_string(mininmumStarsCount[id]);
                    }
                    if (minimumForksCount.contains(id)) {
                        url += "+forks:>=" + std::to_string(minimumForksCount[id]);
                    }
                    if (languageInRepo.contains(id)) {
                        url += "+language:\"" + languageInRepo[id] + "\"";
                    }
                    if (url != "https://api.github.com/search/repositories?q=") {
                        std::cout << url << std::endl;
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

                            if (root.get<int>("total_count") > 0) {
                                std::string response;

                                int count = 1;

                                for (boost::property_tree::ptree::value_type &item: root.get_child("items")) {
                                    response += std::to_string(count) + ") имя: " + item.second.get<std::string>("name")
                                                + ", владелец: " +
                                                item.second.get_child("owner").get<std::string>("login")
                                                + ", url: " + item.second.get<std::string>("html_url") + "\n";
                                    count++;
                                }

                                std::cout << response << std::endl;
                                bot.getApi().sendMessage(id, response, false, 0, keyboard);

                            } else {
                                bot.getApi().sendMessage(id, "Найдено 0 репозиториев с заданными параметрами",
                                                         false, 0, keyboard);
                            }
                        } else {
                            bot.getApi().sendMessage(id, "Не удалось найти репозитории по заданным параметрам",
                                                     false, 0, keyboard);
                        }

                    }
                }
            }
        }
    });
}

void
GithubHelperBot::onCallbackQuerySearchDropAllParameters(TgBot::Bot &bot, TgBot::InlineKeyboardMarkup::Ptr &keyboard) {
    bot.getEvents().onCallbackQuery([&bot, &keyboard, this](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "dropAllParameters")) {
            int64_t id = query->message->chat->id;

            if (content.contains(id)) {
                content.erase(id);
            } else if (user.contains(id)) {
                user.erase(id);
            } else if (repoVisibibillity.contains(id)) {
                repoVisibibillity.erase(id);
            } else if (minimumTopicsCountInRepo.contains(id)) {
                minimumTopicsCountInRepo.erase(id);
            } else if (mininmumStarsCount.contains(id)) {
                mininmumStarsCount.erase(id);
            } else if (minimumForksCount.contains(id)) {
                minimumForksCount.erase(id);
            } else if (languageInRepo.contains(id)) {
                languageInRepo.erase(id);
            }

            bot.getApi().sendMessage(id, "Успешно удалены все параметры для поиска",
                                     false, 0, keyboard);
        }
    });

}

