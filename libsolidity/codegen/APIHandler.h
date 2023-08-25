#pragma once


#include <libsolidity/ast/ASTForward.h>
#include <libsolidity/parsing/Token.h>
#include <libsolidity/ast/Types.h>
#include <libsolidity/ast/ASTAnnotations.h>
#include <libsolidity/ast/ASTEnums.h>

#include <liblangutil/SourceLocation.h>
#include <libevmasm/Instruction.h>

#include <libsolutil/FixedHash.h>
#include <json/json.h>

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace solidity::frontend
{

/// Forward declaration.
class CompilerContext;
class CompilerUtils;

class APIHandler {
private:
	enum APIParameterType : uint8_t {
		kLiteral = 0,
		kIdentifier = 1
	};

	// struct LiteralInfo {
	// 	Token::Value m_Type;
	// 	std::string m_Value;

	// 	LiteralInfo() : m_Type(), m_Value() {}
	// 	explicit LiteralInfo(Token::Value pType, std::string pValue)
	// 		: m_Type(pType), m_Value(pValue) {}
	// };

	// struct IdentifierInfo {
	// 	TypePointer m_Type;
	// 	Declaration const* m_Declaration;
	// 	Expression const* m_Expression;

	// 	IdentifierInfo() : m_Type(), m_Declaration(nullptr), m_Expression(nullptr) {}
	// 	explicit IdentifierInfo(TypePointer pType, Declaration const* pDeclaration, Expression const* pExpression)
	// 		: m_Type(pType), m_Declaration(pDeclaration), m_Expression(pExpression) {}
	// };

	// struct APIParameter {
	// 	APIParameterType m_Type;
	// 	LiteralInfo m_LiteralInfo;
	// 	IdentifierInfo m_IdentifierInfo;
	// };

public:
	APIHandler() :
		m_TypeSection(),
		m_ReturnSection(),
	//	m_FunctionName(),
	//	m_Parameters(),
		m_Context(nullptr) {}

	virtual ~APIHandler() {}
	/// Appends API function parameters
	// void appendLiteral(Token::Value, std::string);
	// void appendIdentifier(TypePointer, Declaration const&, Expression const&);

	/// Clear all API function parameters
	// void clearAPIObjects();

	/// Set current CompilerContext.
	void setContext(CompilerContext* pContext) { m_Context = pContext; }

	/// Pack API parameters to memory.
	// void packedToMemory(Instruction ins, Token::Value retType);

	/// Get CompilerUtils from current CompilerContext.
	CompilerUtils utils();

private:
	/// Initialize size of data section
	/// stack pre: typeSectionOffset
	/// stack post: typeSectionOffset dataSectionOffset
	void initSizeOfDataSection();
	/// Calculate and store size of data section
	/// stack pre: typeSectionOffset dataSectionOffset
	/// stack post: dataSectionOffset typeSectionOffset
	void storeSizeOfDataSection();
	/// Calculate and store size of type section
	/// stack pre: typeSectionOffset
	/// stack post: typeSectionOffset
	void storeSizeOfTypeSection();
	/// Calculate and store size of return section
	/// stack pre: typeSectionOffset
	/// stack post: typeSectionOffset
	void storeSizeOfReturnSection();
	/// Store type section into memory
	/// stack pre: typeSectionOffset
	/// stack post: typeSectionOffset
	void storeTypeSection();
	/// Store return section into memory
	/// stack pre: typeSectionOffset
	/// stack post: typeSectionOffset
	void storeReturnSection();
	/// Handle literal type of parameters
	// void handleLiteral(LiteralInfo&);
	/// Handle identifer type of parameters
	// void handleIdentifier(IdentifierInfo&);

private:
	bytes m_TypeSection;
	bytes m_ReturnSection;
	/// Store current API function name.
	//std::string m_FunctionName;
	/// Store literals and identifiers of API function parameters.
	// std::vector<APIParameter> m_Parameters;
	/// Use CompilerContext to access identifiers.
	CompilerContext* m_Context;
};

}
