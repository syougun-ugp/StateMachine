//-------------------------------------------------
// ��ԊǗ��N���X
// 
// StateMachine : ��Ԃ̊Ǘ�����
// StateBase : ��Ԃ̊��N���X
// StateSwitch : ��Ԃ�؂�ւ���N���X
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

	// �A�b�v�f�[�g�̑O�ɌĂ΂��
	virtual void Start() = 0;

	// �A�b�v�f�[�g
	virtual void Update() = 0;

	// ��Ԃ��ς��Ƃ��̏���
	virtual void OnChangeEvent() = 0;

	// �؂�ւ���������擾
	bool IsNext()const{ return isNext; }

protected:
	// ���ɍs�����߂̏���
	bool isNext;

};


// ��Ԃ̐؂�ւ��N���X
class StateSwitch
{
public:
	StateSwitch() = default;
	StateSwitch(const std::shared_ptr<StateBase> state, const std::string nextRegisterName) :
		state(state), nextRegisterName(nextRegisterName)
	{
	}

	// �A�b�v�f�[�g�̑O�ɌĂ΂��
	void Start()
	{
		state->Start();
	}

	// �A�b�v�f�[�g
	void Update()
	{
		state->Update();
	}

	// ���̏�Ԃɂ����邩�ǂ���
	bool CanNextState()
	{
		if (state->IsNext())
		{
			state->OnChangeEvent();
			return true;
		}
		return false;
	}

	// ���ɍs���o�^�������O
	const std::string nextRegisterName;

private:
	// �e�N���X
	std::shared_ptr<StateBase> state;
};

// ��ԊǗ�
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

	// �o�^����
	void Register(const std::string& name, const std::shared_ptr<StateSwitch> state)
	{
		stateMap.insert(std::make_pair(name, state));
		std::cout << "StateMachine : " << name << "��o�^" << std::endl;
	}

	// �ŏ�����n�߂��Ԃ�ݒ�
	void SetStartState(const std::string& registerName)
	{
		auto it = stateMap.find(registerName);
		if (it == stateMap.end()) return;

		std::cout << "StateMachine : " << it->first << "���X�^�[�g��Ԃɐݒ�" << std::endl;

		nowState = it->second;
		nowState->Start();

	}

	// �o�^�������̂��폜����
	void Deregistration(const std::string& registerName)
	{
		auto it = stateMap.find(registerName);
		if (it == stateMap.end()) return;
		
		std::cout << "StateMachine : " << it->first << "��o�^���폜����" << std::endl;

		stateMap.erase(it);
	}

	// ���ׂĂ̓o�^���폜����
	void AllDeregistration()
	{
		std::cout << "StateMachine : " << "���ׂĂ̓o�^���폜����" << std::endl;

		stateMap.clear();
	}

private:
	std::unordered_map<std::string, std::shared_ptr<StateSwitch>> stateMap;
	std::shared_ptr<StateSwitch> nowState;
};

// �ړ���ԃN���X
class PlayerMover:public StateBase
{
public:
	PlayerMover()
	{

	}

	void Start()
	{
		std::cout << "PlayerMover : " << "�X�^�[�g" << std::endl;
	}

	void Update()
	{
		std::cout << "PlayerMover : " << "�A�b�v�f�[�g" << std::endl;
		isNext = true;
	}

	// ��Ԃ��ς��Ƃ��̏���
	void OnChangeEvent()
	{
		isNext = false;
		std::cout << "PlayerMover : "<< "�؂�ւ��C�x���g�ɓ�����" << std::endl;
	}

private:

};

// �U����ԃN���X
class PlayerAttacker :public StateBase
{
public:
	PlayerAttacker()
	{

	}

	void Start()
	{
		std::cout << "PlayerAttacker : " << "�X�^�[�g" << std::endl;
	}

	void Update()
	{
		std::cout << "PlayerAttacker : " << "�A�b�v�f�[�g" << std::endl;
		isNext = true;
	}

	void OnChangeEvent()
	{
		isNext = false;
		std::cout << "PlayerAttacker : " << "�؂�ւ��C�x���g�ɓ�����" << std::endl;
	}

private:

};

// �~�܂��ԃN���X
class PlayerStoper :public StateBase
{
public:
	PlayerStoper()
	{

	}


	void Start()
	{
		std::cout << "PlayerStoper : " << "�X�^�[�g" << std::endl;
	}

	void Update()
	{
		std::cout << "PlayerStoper : " << "�A�b�v�f�[�g" << std::endl;
		//isNext = true;
	}

	void OnChangeEvent()
	{
		isNext = false;
		std::cout << "PlayerStoper : " << "�؂�ւ��C�x���g�ɓ�����" << std::endl;
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

