#include "Level3.h"

#define L3_ENEMY_COUNT 1

#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8

unsigned int level3_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level3::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("resources/tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    InitPlayer();
    InitEnemies();
    InitMusic();
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, L3_ENEMY_COUNT, state.map);
    if (state.player->kills == L3_ENEMY_COUNT) {
        state.nextScene = 4;
    }
}

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    // render lives
    float x = state.player->position.x;
    float y = state.player->position.y;
    GLuint font_TID = Util::LoadTexture("resources/font1.png");
    Util::DrawText(program, font_TID, "Lives: " + std::to_string(state.player->lives), 0.25, -0.15, glm::vec3(x - 0.25, y + 0.5, 0));
}

void Level3::InitPlayer() {
    state.player = new Entity();
    state.player->entityType = PLAYER;

    state.player->position = glm::vec3(5, 0, 0);
    state.player->savedPoint = glm::vec3(5, 0, 0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->textureID = Util::LoadTexture("resources/george_0.png");

    state.player->speed = 2.0;
    state.player->jumpPower = 6.0f;
    state.player->jumpSound = Mix_LoadWAV("resources/jump.wav");

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    state.player->height = 0.8f;
    state.player->width = 0.8f;
}


void Level3::InitEnemies() {
    state.enemies = new Entity[L3_ENEMY_COUNT];
    GLuint enemy_TID = Util::LoadTexture("resources/ken.png");

    for (int i = 0; i < L3_ENEMY_COUNT; i++) {
        state.enemies[i].textureID = enemy_TID;
        state.enemies[i].acceleration = glm::vec3(0, -9.81, 0);
        state.enemies[i].width = 0.7f;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].speed = 0.75f;
        state.enemies[i].isActive = false;
    }

    // spawn on first jump platform
    state.enemies[0].position = glm::vec3(-1.5, -1.5, 0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    // spawn on second jump platform
    state.enemies[1].position = glm::vec3(2.5, -0.5, 0);
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].jumpPower = 3.0f;
    // spawn on ground floor
    state.enemies[2].position = glm::vec3(4, -2.5, 0);
    state.enemies[2].aiType = CHASER;
    state.enemies[2].aiState = CHASING;

}

void Level3::InitMusic() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    state.music = Mix_LoadMUS("resources/wholesome.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
}