
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// ======================================
// FoxYFua Circular Button
// ======================================

static CCNode* createCircleButton(const char* text) {
    auto circle = CCDrawNode::create();
    if (!circle) return nullptr;

    const float radius = 27.f;

    circle->drawDot(
        ccp(radius, radius),
        radius,
        ccc4f(0.f, 0.85f, 1.f, 1.f)
    );

    circle->drawDot(
        ccp(radius, radius),
        radius - 3.f,
        ccc4f(0.025f, 0.08f, 0.13f, 1.f)
    );

    auto label = CCLabelBMFont::create(text, "bigFont.fnt");

    if (label) {
        label->setScale(0.42f);
        label->setPosition(radius, radius);
        circle->addChild(label);
    }

    circle->setContentSize({54.f, 54.f});
    return circle;
}

// ======================================
// FoxYFua Hub
// ======================================

class FoxYFuaHub : public CCLayer {
protected:
    CCLayerColor* m_panel = nullptr;
    CCLayerColor* m_border = nullptr;
    CCLabelBMFont* m_status = nullptr;

    bool m_glass = true;
    bool m_cyan = true;

    bool init() {
        if (!CCLayer::init())
            return false;

        auto win = CCDirector::sharedDirector()->getWinSize();

        // Load saved settings
        m_glass = Mod::get()->getSavedValue<bool>(
            "ui-glass", true
        );

        m_cyan = Mod::get()->getSavedValue<bool>(
            "ui-cyan", true
        );

        float width = win.width * 0.90f;
        float height = win.height * 0.85f;

        // Main panel
        m_panel = CCLayerColor::create(
            ccc4(5, 18, 30, m_glass ? 205 : 245),
            width,
            height
        );

        if (!m_panel)
            return false;

        m_panel->setPosition({
            (win.width - width) / 2.f,
            (win.height - height) / 2.f
        });

        this->addChild(m_panel);

        // Cyan border
        m_border = CCLayerColor::create(
            ccc4(0, 220, 255, 255),
            width,
            3.f
        );

        m_border->setPosition(0, height - 3.f);
        m_border->setColor(
            m_cyan ? ccc3(0, 220, 255)
                   : ccc3(190, 190, 190)
        );

        m_panel->addChild(m_border);

        // Title
        auto title = CCLabelBMFont::create(
            "FoxYFua MEGA HUB",
            "bigFont.fnt"
        );

        title->setScale(0.7f);
        title->setColor({0, 220, 255});
        title->setPosition(width / 2.f, height - 35.f);
        m_panel->addChild(title);

        auto subtitle = CCLabelBMFont::create(
            "GLASS EDITION",
            "goldFont.fnt"
        );

        subtitle->setScale(0.4f);
        subtitle->setPosition(width / 2.f, height - 58.f);
        m_panel->addChild(subtitle);

        // Close button
        auto closeSprite = createCircleButton("X");

        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(FoxYFuaHub::onClose)
        );

        auto closeMenu = CCMenu::create();
        closeMenu->addChild(closeButton);

        closeMenu->setPosition({
            width - 34.f,
            height - 34.f
        });

        m_panel->addChild(closeMenu);

        // Divider
        auto divider = CCLayerColor::create(
            ccc4(0, 190, 230, 180),
            width - 35.f,
            2.f
        );

        divider->setPosition(17.f, height - 80.f);
        m_panel->addChild(divider);

        // Categories
        auto categoryTitle = CCLabelBMFont::create(
            "CATEGORIES",
            "goldFont.fnt"
        );

        categoryTitle->setScale(0.45f);
        categoryTitle->setColor({0, 220, 255});
        categoryTitle->setAnchorPoint({0.f, 0.5f});
        categoryTitle->setPosition(25.f, height - 110.f);

        m_panel->addChild(categoryTitle);

        const char* categories =
            "HOME\n"
            "PLAYER\n"
            "SPEED\n"
            "VISUAL\n"
            "CREATOR\n"
            "MACRO\n"
            "AUTO CLICKER\n"
            "SOUND\n"
            "SETTINGS";

        auto categoryLabel = CCLabelBMFont::create(
            categories,
            "bigFont.fnt"
        );

        categoryLabel->setScale(0.43f);
        categoryLabel->setAnchorPoint({0.f, 1.f});
        categoryLabel->setColor({190, 235, 255});
        categoryLabel->setPosition(25.f, height - 135.f);

        m_panel->addChild(categoryLabel);

        // Main content
        auto welcome = CCLabelBMFont::create(
            "WELCOME TO FOX YFUA!\n\n"
            "Your Geometry Dash control hub.\n\n"
            "Choose a category to continue.\n\n"
            "More functional features are coming.",
            "goldFont.fnt"
        );

        welcome->setScale(0.48f);
        welcome->setColor({220, 245, 255});
        welcome->setAlignment(kCCTextAlignmentCenter);

        welcome->setPosition({
            width * 0.66f,
            height * 0.65f
        });

        m_panel->addChild(welcome);

        // UI settings title
        auto settingsTitle = CCLabelBMFont::create(
            "QUICK UI SETTINGS",
            "goldFont.fnt"
        );

        settingsTitle->setScale(0.48f);
        settingsTitle->setColor({0, 220, 255});
        settingsTitle->setPosition({
            width * 0.66f,
            height * 0.38f
        });

        m_panel->addChild(settingsTitle);

        // Glass toggle button
        auto glassLabel = CCLabelBMFont::create(
            "TOGGLE GLASS TRANSPARENCY",
            "bigFont.fnt"
        );

        glassLabel->setScale(0.42f);

        auto glassButton = CCMenuItemLabel::create(
            glassLabel,
            this,
            menu_selector(FoxYFuaHub::onToggleGlass)
        );

        // Cyan toggle button
        auto cyanLabel = CCLabelBMFont::create(
            "TOGGLE CYAN ACCENT",
            "bigFont.fnt"
        );

        cyanLabel->setScale(0.42f);

        auto cyanButton = CCMenuItemLabel::create(
            cyanLabel,
            this,
            menu_selector(FoxYFuaHub::onToggleCyan)
        );

        auto settingsMenu = CCMenu::create();
        settingsMenu->addChild(glassButton);
        settingsMenu->addChild(cyanButton);

        settingsMenu->alignItemsVerticallyWithPadding(18.f);

        settingsMenu->setPosition({
            width * 0.66f,
            height * 0.25f
        });

        m_panel->addChild(settingsMenu);

        // Status
        m_status = CCLabelBMFont::create(
            "",
            "goldFont.fnt"
        );

        m_status->setScale(0.38f);
        m_status->setColor({0, 220, 255});
        m_status->setPosition({
            width * 0.66f,
            25.f
        });

        m_panel->addChild(m_status);

        updateStatus();

        return true;
    }

    void updateStatus() {
        if (!m_status)
            return;

        std::string status =
            std::string("GLASS: ") +
            (m_glass ? "ON" : "OFF") +
            "  |  CYAN: " +
            (m_cyan ? "ON" : "OFF");

        m_status->setString(status.c_str());
    }

    void onClose(CCObject*) {
        this->setVisible(false);
    }

    void onToggleGlass(CCObject*) {
        m_glass = !m_glass;

        Mod::get()->setSavedValue<bool>(
            "ui-glass", m_glass
        );

        m_panel->setOpacity(
            m_glass ? 205 : 245
        );

        updateStatus();
    }

    void onToggleCyan(CCObject*) {
        m_cyan = !m_cyan;

        Mod::get()->setSavedValue<bool>(
            "ui-cyan", m_cyan
        );

        m_border->setColor(
            m_cyan ? ccc3(0, 220, 255)
                   : ccc3(190, 190, 190)
        );

        updateStatus();
    }

public:
    static FoxYFuaHub* create() {
        auto ret = new FoxYFuaHub();

        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ======================================
// Main Menu Hook
// ======================================

class $modify(FoxyFuaMenuLayer, MenuLayer) {
    struct Fields {
        Ref<FoxYFuaHub> hub;
    };

    bool init() {
        if (!MenuLayer::init())
            return false;

        auto win = CCDirector::sharedDirector()->getWinSize();

        // Create Hub
        auto hub = FoxYFuaHub::create();

        if (hub) {
            this->addChild(hub, 1000);
            hub->setVisible(false);
            m_fields->hub = hub;
        }

        // Floating circular FOX button
        auto foxSprite = createCircleButton("FOX");

        auto openButton = CCMenuItemSpriteExtra::create(
            foxSprite,
            this,
            menu_selector(FoxyFuaMenuLayer::onToggleHub)
        );

        auto menu = CCMenu::create();
        menu->addChild(openButton);

        menu->setPosition({
            win.width - 42.f,
            42.f
        });

        this->addChild(menu, 1001);

        return true;
    }

    void onToggleHub(CCObject*) {
        if (!m_fields->hub)
            return;

        m_fields->hub->setVisible(
            !m_fields->hub->isVisible()
        );
    }
};
