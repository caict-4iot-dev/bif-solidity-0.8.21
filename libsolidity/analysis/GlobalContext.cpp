/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/**
 * @author Christian <c@ethdev.com>
 * @author Gav Wood <g@ethdev.com>
 * @date 2014
 * Container of the (implicit and explicit) global objects.
 */

#include <libsolidity/analysis/GlobalContext.h>

#include <libsolidity/ast/AST.h>
#include <libsolidity/ast/TypeProvider.h>
#include <libsolidity/ast/Types.h>
#include <memory>

using namespace std;

namespace solidity::frontend
{

namespace
{
/// Magic variables get negative ids for easy differentiation
int magicVariableToID(std::string const& _name)
{
	if (_name == "abi") return -1;
	else if (_name == "addmod") return -2;
	else if (_name == "assert") return -3;
	else if (_name == "block") return -4;
	else if (_name == "blockhash") return -5;
	else if (_name == "ecrecover") return -6;
	else if (_name == "gasleft") return -7;
	else if (_name == "keccak256") return -8;
	else if (_name == "msg") return -15;
	else if (_name == "mulmod") return -16;
	else if (_name == "now") return -17;
	else if (_name == "require") return -18;
	else if (_name == "revert") return -19;
	else if (_name == "ripemd160") return -20;
	else if (_name == "selfdestruct") return -21;
	else if (_name == "sha256") return -22;
	else if (_name == "sha3") return -23;
	else if (_name == "suicide") return -24;
	else if (_name == "super") return -25;
	else if (_name == "tx") return -26;
	else if (_name == "type") return -27;
	else if (_name == "this") return -28;
	else if (_name == "addrprefix") return -29;
	else if (_name == "test1") return -30;
	else if (_name == "stoi64Check") return -31;
	else if (_name == "ecVerify") return -32;
	else if (_name == "toAddress") return -33;
	else if (_name == "bcRangeProofVerify") return -34;
	else if (_name == "pedersenTallyVerify") return -35;
	else if (_name == "sender") return -36;
	else if (_name == "initiator") return -37;
	else if (_name == "operationIndex") return -38;
	else if (_name == "noice") return -39;
	else if (_name == "asset") return -40;
	else if (_name == "feeLimit") return -41;
	else if (_name == "accountPrivilege") return -42;
	else if (_name == "trust") return -43;
	else if (_name == "status") return -44;
	else if (_name == "controlledAttr") return -45;
	else if (_name == "setAccountTrusted") return -46;
	else if (_name == "setAccountStatus") return -47;
	else if (_name == "setControlledAttr") return -48;
	else if (_name == "issueAsset") return -49;
	else if (_name == "payAsset") return -50;
	else if (_name == "sdel") return -51;
	else if (_name == "isValidator") return -52;
	else if (_name == "getValidators") return -53;
	else if (_name == "quorumSize") return -54;
	else if (_name == "configFee") return -55;
	else if (_name == "setValidators") return -56;
	else if (_name == "addressCheck") return -57;
	else
		solAssert(false, "Unknown magic variable: \"" + _name + "\".");
}

inline vector<shared_ptr<MagicVariableDeclaration const>> constructMagicVariables()
{
	static auto const magicVarDecl = [](string const& _name, Type const* _type) {
		return make_shared<MagicVariableDeclaration>(magicVariableToID(_name), _name, _type);
	};

	return {
		magicVarDecl("abi", TypeProvider::magic(MagicType::Kind::ABI)),
		magicVarDecl("addmod", TypeProvider::function(strings{"uint256", "uint256", "uint256"}, strings{"uint256"}, FunctionType::Kind::AddMod, StateMutability::Pure)),
		magicVarDecl("assert", TypeProvider::function(strings{"bool"}, strings{}, FunctionType::Kind::Assert, StateMutability::Pure)),
		magicVarDecl("block", TypeProvider::magic(MagicType::Kind::Block)),
		magicVarDecl("blockhash", TypeProvider::function(strings{"uint256"}, strings{"bytes32"}, FunctionType::Kind::BlockHash, StateMutability::View)),
		magicVarDecl("ecrecover", TypeProvider::function(strings{"bytes32", "uint8", "bytes32", "bytes32"}, strings{"address"}, FunctionType::Kind::ECRecover, StateMutability::Pure)),
		magicVarDecl("gasleft", TypeProvider::function(strings(), strings{"uint256"}, FunctionType::Kind::GasLeft, StateMutability::View)),
		magicVarDecl("keccak256", TypeProvider::function(strings{"bytes memory"}, strings{"bytes32"}, FunctionType::Kind::KECCAK256, StateMutability::Pure)),
		magicVarDecl("msg", TypeProvider::magic(MagicType::Kind::Message)),
		magicVarDecl("mulmod", TypeProvider::function(strings{"uint256", "uint256", "uint256"}, strings{"uint256"}, FunctionType::Kind::MulMod, StateMutability::Pure)),
		magicVarDecl("now", TypeProvider::uint256()),
		magicVarDecl("require", TypeProvider::function(strings{"bool"}, strings{}, FunctionType::Kind::Require, StateMutability::Pure)),
		magicVarDecl("require", TypeProvider::function(strings{"bool", "string memory"}, strings{}, FunctionType::Kind::Require, StateMutability::Pure)),
		magicVarDecl("revert", TypeProvider::function(strings(), strings(), FunctionType::Kind::Revert, StateMutability::Pure)),
		magicVarDecl("revert", TypeProvider::function(strings{"string memory"}, strings(), FunctionType::Kind::Revert, StateMutability::Pure)),
		magicVarDecl("ripemd160", TypeProvider::function(strings{"bytes memory"}, strings{"bytes20"}, FunctionType::Kind::RIPEMD160, StateMutability::Pure)),
		magicVarDecl("selfdestruct", TypeProvider::function(strings{"address payable"}, strings{}, FunctionType::Kind::Selfdestruct)),
		magicVarDecl("sha256", TypeProvider::function(strings{"bytes memory"}, strings{"bytes32"}, FunctionType::Kind::SHA256, StateMutability::Pure)),
		//add by zhangbo
		magicVarDecl("addrprefix", TypeProvider::function(strings{ "bytes32" }, strings{ "string memory" }, FunctionType::Kind::AddrPrefix, StateMutability::Pure)),

		//add by zhangbo
		magicVarDecl("test1", TypeProvider::function(strings(), strings{ "string" }, FunctionType::Kind::Test1, StateMutability::Pure)),

		/**********************add by zhangbo*********************/
		magicVarDecl("stoi64Check", TypeProvider::function(strings{ "bytes32" }, strings{ "bool" }, FunctionType::Kind::Stoi64Check, StateMutability::Pure)),
		magicVarDecl("ecVerify", TypeProvider::function(strings{ "string memory","string memory","string memory", "uint8" }, strings{ "bool" }, FunctionType::Kind::EcVerify, StateMutability::Pure)),
		magicVarDecl("toAddress", TypeProvider::function(strings{ "string memory"}, strings{ "string memory" }, FunctionType::Kind::ToAddress, StateMutability::Pure)),
		magicVarDecl("bcRangeProofVerify", TypeProvider::function(strings{ "string memory",  "string memory" }, strings{ "bool" }, FunctionType::Kind::BcRangeProofVerify, StateMutability::Pure)),
		magicVarDecl("pedersenTallyVerify", TypeProvider::function(strings{ "string memory",  "string memory", "bytes32",  "string memory" }, strings{ "bool" }, FunctionType::Kind::PedersenTallyVerify, StateMutability::Pure)),

		magicVarDecl("sender", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::Sender, StateMutability::Pure)),
		magicVarDecl("initiator", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::Initiator, StateMutability::Pure)),
		magicVarDecl("operationIndex", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::OperationIndex, StateMutability::Pure)),
		magicVarDecl("noice", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::Noice, StateMutability::Pure)),
		magicVarDecl("asset", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::Assert, StateMutability::Pure)),
		magicVarDecl("feeLimit", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::FeeLimit, StateMutability::Pure)),
		magicVarDecl("accountPrivilege", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::AccountPrivilege, StateMutability::Pure)),
		magicVarDecl("trust", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::Trust, StateMutability::Pure)),
		magicVarDecl("status", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::Status, StateMutability::Pure)),
		magicVarDecl("controlledAttr", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::ControlledAttr, StateMutability::Pure)),
		magicVarDecl("setAccountTrusted", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::SetAccountTrusted, StateMutability::Pure)),
		magicVarDecl("setAccountStatus", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::SetAccountStatus, StateMutability::Pure)),
		magicVarDecl("setControlledAttr", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::SetControlledAttr, StateMutability::Pure)),
		magicVarDecl("issueAsset", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::IssueAsset, StateMutability::Pure)),
		magicVarDecl("payAsset", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::PayAsset, StateMutability::Pure)),
		magicVarDecl("sdel", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::SDel, StateMutability::Pure)),
		magicVarDecl("isValidator", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::IsValidator, StateMutability::Pure)),
		magicVarDecl("getValidators", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::GetValidators, StateMutability::Pure)),
		magicVarDecl("quorumSize", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::QuorumSize, StateMutability::Pure)),
		magicVarDecl("configFee", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::ConfigFee, StateMutability::Pure)),
		magicVarDecl("setValidators", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::SetValidators, StateMutability::Pure)),
		magicVarDecl("addressCheck", TypeProvider::function(strings{ "string memory", "uint8" }, strings{ "string memory" }, FunctionType::Kind::AddressCheck, StateMutability::Pure)),
		/******************************************************/
		
		magicVarDecl("sha3", TypeProvider::function(strings{"bytes memory"}, strings{"bytes32"}, FunctionType::Kind::KECCAK256, StateMutability::Pure)),
		magicVarDecl("suicide", TypeProvider::function(strings{"address payable"}, strings{}, FunctionType::Kind::Selfdestruct)),
		magicVarDecl("tx", TypeProvider::magic(MagicType::Kind::Transaction)),
		// Accepts a MagicType that can be any contract type or an Integer type and returns a
		// MagicType. The TypeChecker handles the correctness of the input and output types.
		magicVarDecl("type", TypeProvider::function(
			strings{},
			strings{},
			FunctionType::Kind::MetaType,
			StateMutability::Pure,
			FunctionType::Options::withArbitraryParameters()
		)),
	};
}

}

GlobalContext::GlobalContext(): m_magicVariables{constructMagicVariables()}
{
}

void GlobalContext::setCurrentContract(ContractDefinition const& _contract)
{
	m_currentContract = &_contract;
}

vector<Declaration const*> GlobalContext::declarations() const
{
	vector<Declaration const*> declarations;
	declarations.reserve(m_magicVariables.size());
	for (ASTPointer<MagicVariableDeclaration const> const& variable: m_magicVariables)
		declarations.push_back(variable.get());
	return declarations;
}

MagicVariableDeclaration const* GlobalContext::currentThis() const
{
	if (!m_thisPointer[m_currentContract])
	{
		Type const* type = TypeProvider::emptyTuple();
		if (m_currentContract)
			type = TypeProvider::contract(*m_currentContract);
		m_thisPointer[m_currentContract] =
			make_shared<MagicVariableDeclaration>(magicVariableToID("this"), "this", type);
	}
	return m_thisPointer[m_currentContract].get();
}

MagicVariableDeclaration const* GlobalContext::currentSuper() const
{
	if (!m_superPointer[m_currentContract])
	{
		Type const* type = TypeProvider::emptyTuple();
		if (m_currentContract)
			type = TypeProvider::typeType(TypeProvider::contract(*m_currentContract, true));
		m_superPointer[m_currentContract] =
			make_shared<MagicVariableDeclaration>(magicVariableToID("super"), "super", type);
	}
	return m_superPointer[m_currentContract].get();
}

}
