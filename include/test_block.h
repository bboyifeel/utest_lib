#pragma once

#include "container_template.h"

namespace xTest
{

//**************************************************************************************************
//	eTestBlock
//--------------------------------------------------------------------------------------------------
class TEST_API eTestBlock
{
protected:
	class eResultLoader;
	enum eState
	{
		S_OPENED = 0,
		S_COMPLETED,
		S_STARTING,		// S_LAST
	};
	enum
	{
		INVALID_RESULT = INT32_MIN
	};
public:
	eTestBlock(const std::string& _name = "") : name(_name) {}
	virtual ~eTestBlock() = default;

	virtual void		Done()			{}
	virtual bool		Start();

	virtual bool		IsCompleted()		const		{ return state == S_COMPLETED;		}
	const std::string&	Name()				const		{ return name;						}

	virtual bool		Load(json&);

	inline	int32_t		ResultExpected()	const		{ return (int32_t)resultExpected;	}
	inline	void		ResultExpected(int32_t result)	{ resultExpected = result;			}
	inline	int32_t		ResultActual()		const		{ return (int32_t)resultActual;		}					
	inline	void		ResultActual(int32_t result)												
	{																						
		if (resultActual != result)																
		{																					
			resultActual = result;															
			OnResultActual();																
		}																					
	}

protected:
	virtual	void			OnResultActual();
	virtual bool			LoadIdent(json&);
	virtual eResultLoader*	ResultLoader()	const		{ return new eResultLoader;			}

	void				State(int32_t);
	virtual void		NextState(int32_t oldState)		{}

	template <typename TEST_RESULT_ENUM>
	inline void			ResultActual(TEST_RESULT_ENUM _result)	{ ResultActual((int32_t)_result); }

	std::string			name;
	int32_t				state			= S_OPENED;
	int32_t				resultActual	= INVALID_RESULT;
	int32_t				resultExpected	= INVALID_RESULT;
	eResultLoader*		resultLoader	= nullptr;

protected:
	//==============================================================================================
	//	eResultLoader
	//----------------------------------------------------------------------------------------------
	class TEST_API eResultLoader
	{
	public:
		virtual ~eResultLoader() = default;
		virtual bool	LoadResult(json&, eTestBlock*);
	};
	//==============================================================================================
	//	eNamedResultLoader
	//----------------------------------------------------------------------------------------------
// 	template <typename T>
// 	class eNamedResultLoader : public eResultLoader
// 	{
// 		using eInherited = eResultLoader;
// 	public:
// 		virtual ~eNamedResultLoader() = default;
// 		virtual bool LoadResult(json& _jFile, eTestBlock* _testBlock)
// 		{
// 			if(_is.Test(ltString))
// 			{
// 				T result = FromString<T>(_is.GetString());
// 				if(result == T::INVALID)
// 				{
// 					LSD_WARNING("[eNamedResultLoader] Load fail: undefined result value");
// 					return false;
// 				}
// 				_testBlock->ResultExpected((int32_t)result);
// 				return true;
// 			}
// 			return eInherited::LoadResult(_is, _testBlock);
// 		}
// 	};
};

class TEST_API eTestBlocks_t : public eVectorGeneric<eTestBlock>
{};

}//namespace xTest
