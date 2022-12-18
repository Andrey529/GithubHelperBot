#include <iostream>
#include "src/GithubHelperBot.h"

int main() {
    std::string token(std::getenv("TGBOT_TOKEN"));
    GithubHelperBot bot(token);
    return 0;
}
