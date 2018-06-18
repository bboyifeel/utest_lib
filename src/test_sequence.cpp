#include "stdafx.h"
#include "test_sequence.h"
#include "test_block.h"
#include "test_blocks.h"

namespace xTest
{

//**************************************************************************************************
//	eTestSequence::eTestSequence
//--------------------------------------------------------------------------------------------------
eTestSequence::eTestSequence()
{
	blocks = new eTestBlocks_t;
}
//==================================================================================================
//	eTestSequence::~eTestSequence
//--------------------------------------------------------------------------------------------------
eTestSequence::~eTestSequence()
{
	blocks->Clear();
	xBase::SAFE_DELETE(blocks);
}
//==================================================================================================
//	eTestSequence::Start
//--------------------------------------------------------------------------------------------------
bool eTestSequence::Start(const eTestRunner& _runner)
{
	if(blocks->empty()) return false;
	CurrentStart();
	return true;
}
//==================================================================================================
//	eTestSequence::Update
//--------------------------------------------------------------------------------------------------
bool eTestSequence::Update()
{
	return IsCompleted() || UpdateCurrent();
}
//==================================================================================================
//	eTestSequence::UpdateCurrent
//--------------------------------------------------------------------------------------------------
bool eTestSequence::UpdateCurrent()
{
	eTestBlock* currentBlock = Current();
	if(currentBlock && currentBlock->IsCompleted())
	{
		if(currentBlock->ResultExpected() != currentBlock->ResultActual())
		{
			std::string message = "[eTestSequence] UpdateCurrent fail: Test \"";
						message += currentBlock->Name();
						message += "\" is failed: expected \"";
						message += std::to_string(currentBlock->ResultExpected());
						message += "\" != actual \"";
						message += std::to_string(currentBlock->ResultActual());
						message += "\"";
			xIO::Log::Error(message);
			return false;
		}

		++current;
		CurrentStart();
	}
	return true;
}
//==================================================================================================
//	eTestSequence::Current
//--------------------------------------------------------------------------------------------------
eTestBlock* eTestSequence::Current() const
{
	return current < blocks->size() ? &blocks->at(current) : nullptr;
}
//==================================================================================================
//	eTestSequence::CurrentStart
//--------------------------------------------------------------------------------------------------
void eTestSequence::CurrentStart()
{
	eTestBlock* current = Current();
	if(current)
		current->Start();
}
//==================================================================================================
//	eTestSequence::IsCompleted
//--------------------------------------------------------------------------------------------------
bool eTestSequence::IsCompleted() const
{
	return blocks && current == blocks->size();
}
//==================================================================================================
//	eTestSequence::Load
//--------------------------------------------------------------------------------------------------
bool eTestSequence::Load(json& _jFile)
{
	return LoadItems(_jFile["sequence"]);
}
//==================================================================================================
//	eTestSequence::ConfigFileName
//--------------------------------------------------------------------------------------------------
void eTestSequence::ConfigFileName(const std::string& _fName)
{
	configFileName = _fName;
}
//==================================================================================================
//	eTestSequence::LoadItems
//--------------------------------------------------------------------------------------------------
bool eTestSequence::LoadItems(json& _jFile)
{
	for(int i = 0; i < _jFile.size(); i++)
	{
		if(!_jFile[i]["blocks"].is_null())
		{
			if(!LoadBlocks(_jFile[i]["blocks"]))
				return false;
		}
		else if(!_jFile[i]["block"].is_null())
		{
			eTestBlock* block = eTestBlocks::LoadBlock(_jFile[i]["block"], configFileName);
			if(!block)
			{
				return false;
			}
			blocks->Add(block);
		}
		else
		{
			std::string message = "[eTestSequence] ";
						message += "\"";
						message += configFileName;
						message += "\"";
						message += " LoadItems fail: Undefined block ident";
			xIO::Log::Error(message);
			return false;
		}
	}
	return !blocks->empty();
}
//==================================================================================================
//	eTestSequence::LoadBlocks
//--------------------------------------------------------------------------------------------------
bool eTestSequence::LoadBlocks(json& _jFile)
{
	eTestBlock* block = new eTestBlocks;
	block->ConfigFileName(configFileName);
	bool r = block->Load(_jFile);
	if(r)
	{
		blocks->Add(block);
	}
	return r;
}

//**************************************************************************************************
//	eTestSequences::Start
//--------------------------------------------------------------------------------------------------
bool eTestSequences::Start(const eTestRunner& _runner)
{
	for(eTestSequence* s : *this)
	{
		if(!s->Start(_runner))
		{
			return false;
		}
	}
	return true;
}
//==================================================================================================
//	eTestSequences::Update
//--------------------------------------------------------------------------------------------------
bool eTestSequences::Update()
{
	for(eTestSequence* s : *this)
	{
		if(!s->Update())
		{
			return false;
		}
	}
	return true;
}
//==================================================================================================
//	eTestSequences::IsCompleted
//--------------------------------------------------------------------------------------------------
bool eTestSequences::IsCompleted() const
{
	for(eTestSequence* s : *this)
	{
		if(!s->IsCompleted())
		{
			return false;
		}
	}
	return true;
}
//==================================================================================================
//	eTestSequences::Load
//--------------------------------------------------------------------------------------------------
bool eTestSequences::Load(json& _jFile)
{
	for(int i = 0; i < _jFile.size(); i++)
	{
		eTestSequence* sequence = nullptr;
		if(!LoadItems(_jFile[i], sequence))
		{
			return false;
		}
		if(!sequence || !sequence->Load(_jFile[i]))
		{
			return false;
		}
		Add(sequence);
	}

	return true;
}
//==================================================================================================
//	eTestSequences::ConfigFileName
//--------------------------------------------------------------------------------------------------
void eTestSequences::ConfigFileName(const std::string& _fName)
{
	configFileName = _fName;
}
//==================================================================================================
//	eTestSequences::LoadItems
//--------------------------------------------------------------------------------------------------
bool eTestSequences::LoadItems(json& _jFile, eTestSequence*& _sequence)
{
	if(!_jFile["sequence"].is_null())
	{
		_sequence = new eTestSequence;
		_sequence->ConfigFileName(configFileName);
		return true;
	}
	std::string message = "[eTestSequences] ";
				message += "\"";
				message += configFileName;
				message += "\"";
				message += " LoadIdent fail:";
				message += " Couldn't find ident - sequence";
	xIO::Log::Error(message);
	return false;
}

}//namespace xTest