#include "Scene.h"
class Level1 : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
	void InitPlayer() override;
	void InitEnemies() override;
	void InitMusic() override;
	void InitCoins() override;
	void CreateEnemyCords();
	void CreateCoinCords();
};