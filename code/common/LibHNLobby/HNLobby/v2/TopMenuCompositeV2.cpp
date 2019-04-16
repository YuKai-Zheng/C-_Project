#include "TopMenuCompositeV2.h"

bool TopMenuCompositeV2::init() {
    if (!TopMenuComposite::init()) {
        return false;
    }
    return true;
}

void TopMenuCompositeV2::setType(MenuType type) {
    TopMenuComposite::setType(type);
    if (type == game_list_type) {
        canDisplayButton(_lobbyTop.Button_Return, true);
        canDisplayButton(_lobbyTop.Button_Exit, false);
    } else if (type == game_type_type) {
        canDisplayButton(_lobbyTop.Button_Return, false);
        canDisplayButton(_lobbyTop.Button_Exit, true);
    }
}
