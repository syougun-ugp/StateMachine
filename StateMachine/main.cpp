//-------------------------------------------------
// 状態管理クラス
// 
// StateMachine : 状態の管理部分
// StateBase : 状態の基底クラス
// StateSwitch : 状態を切り替えるクラス
//--------------------------------------------------


#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>


class StateBase
{
public:
	StateBase() :
		isNext(false)
	{

	};

	// アップデートの前に呼ばれる
	virtual void Start() = 0;

	// アップデート
	virtual void Update() = 0;

	// 状態が変わるときの処理
	virtual void OnChangeEvent() = 0;

	// 切り替える条件を取得
	bool IsNext()const{ return isNext; }

protected:
	// 次に行くための条件
	bool isNext;

};


// 状態の切り替えクラス
class StateSwitch
{
public:
	StateSwitch() = default;
	StateSwitch(const std::shared_ptr<StateBase> state, const std::string nextRegisterName) :
		state(state), nextRegisterName(nextRegisterName)
	{
	}

	// アップデートの前に呼ばれる
	void Start()
	{
		state->Start();
	}

	// アップデート
	void Update()
	{
		state->Update();
	}

	// 次の状態にいけるかどうか
	bool CanNextState()
	{
		if (state->IsNext())
		{
			state->OnChangeEvent();
			return true;
		}
		return false;
	}

	// 次に行く登録した名前
	const std::string nextRegisterName;

private:
	// 親クラス
	std::shared_ptr<StateBase> state;
};

// 状態管理
class StateMachine
{
public:
	StateMachine():
		nowState()
	{

	}

	void Update()
	{
		nowState->Update();
		if (nowState->CanNextState())
		{
			auto it = stateMap.find(nowState->nextRegisterName);
			if (it == stateMap.end()) return;

			nowState = it->second;
			nowState->Start();
		}
	}

	// 登録する
	void Register(const std::string& name, const std::shared_ptr<StateSwitch> state)
	{
		stateMap.insert(std::make_pair(name, state));
		std::cout << "StateMachine : " << name << "を登録" << std::endl;
	}

	// 最初から始める状態を設定
	void SetStartState(const std::string& registerName)
	{
		auto it = stateMap.find(registerName);
		if (it == stateMap.end()) return;

		std::cout << "StateMachine : " << it->first << "をスタート状態に設定" << std::endl;

		nowState = it->second;
		nowState->Start();

	}

	// 登録したものを削除する
	void Deregistration(const std::string& registerName)
	{
		auto it = stateMap.find(registerName);
		if (it == stateMap.end()) return;
		
		std::cout << "StateMachine : " << it->first << "を登録を削除する" << std::endl;

		stateMap.erase(it);
	}

	// すべての登録を削除する
	void AllDeregistration()
	{
		std::cout << "StateMachine : " << "すべての登録を削除する" << std::endl;

		stateMap.clear();
	}

private:
	std::unordered_map<std::string, std::shared_ptr<StateSwitch>> stateMap;
	std::shared_ptr<StateSwitch> nowState;
};

// 移動状態クラス
class PlayerMover:public StateBase
{
public:
	PlayerMover()
	{

	}

	void Start()
	{
		std::cout << "PlayerMover : " << "スタート" << std::endl;
	}

	void Update()
	{
		std::cout << "PlayerMover : " << "アップデート" << std::endl;
		isNext = true;
	}

	// 状態が変わるときの処理
	void OnChangeEvent()
	{
		isNext = false;
		std::cout << "PlayerMover : "<< "切り替わるイベントに入った" << std::endl;
	}

private:

};

// 攻撃状態クラス
class PlayerAttacker :public StateBase
{
public:
	PlayerAttacker()
	{

	}

	void Start()
	{
		std::cout << "PlayerAttacker : " << "スタート" << std::endl;
	}

	void Update()
	{
		std::cout << "PlayerAttacker : " << "アップデート" << std::endl;
		isNext = true;
	}

	void OnChangeEvent()
	{
		isNext = false;
		std::cout << "PlayerAttacker : " << "切り替わるイベントに入った" << std::endl;
	}

private:

};

// 止まる状態クラス
class PlayerStoper :public StateBase
{
public:
	PlayerStoper()
	{

	}


	void Start()
	{
		std::cout << "PlayerStoper : " << "スタート" << std::endl;
	}

	void Update()
	{
		std::cout << "PlayerStoper : " << "アップデート" << std::endl;
		//isNext = true;
	}

	void OnChangeEvent()
	{
		isNext = false;
		std::cout << "PlayerStoper : " << "切り替わるイベントに入った" << std::endl;
	}

private:

};

class Player
{
public:
	Player():
		stateMachine(std::make_unique<StateMachine>())
	{
		const std::string moveName = "PlayerMove";
		const std::string attackName = "PlayerAttack";
		const std::string stopName = "PlayerStop";

		auto moveState = std::make_shared<StateSwitch>(std::make_shared<PlayerMover>(), attackName);
		auto attackState = std::make_shared<StateSwitch>(std::make_shared<PlayerAttacker>(), stopName);
		auto stopState = std::make_shared<StateSwitch>(std::make_shared<PlayerStoper>(), moveName);

		stateMachine->Register(moveName, moveState);
		stateMachine->Register(attackName, attackState);
		stateMachine->Register(stopName, stopState);

		stateMachine->SetStartState(moveName);

	}

	void Update()
	{
		stateMachine->Update();
	}

private:
	std::unique_ptr<StateMachine> stateMachine;
};



int main()
{
	std::unique_ptr<Player> player;
	player = std::make_unique<Player>();

	player->Update();
	player->Update();
	player->Update();
	player->Update();

	return 0;
}

