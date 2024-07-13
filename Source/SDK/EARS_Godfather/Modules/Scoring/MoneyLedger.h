#pragma once

// SDK (Common)
#include "SDK/EARS_Common/StaticArray.h"

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		enum LedgerItemType : uint32_t
		{
			LedgerItemType_REF = 0xFFFFFFFF,
			LEDGERITEMTYPE_INVALID = 0xFFFFFFFF,
			LEDGERITEMTYPE_REVENUE_RACKETS = 0x0,
			LEDGERITEMTYPE_REVENUE_EXTORTION = 0x1,
			LEDGERITEMTYPE_REVENUE_FRONTBONUSES = 0x2,
			LEDGERITEMTYPE_REVENUE_DONCONTROL = 0x3,
			LEDGERITEMTYPE_REVENUE_OTHER = 0x4,
			LEDGERITEMTYPE_EXPENSE_BRIBES = 0x5,
			LEDGERITEMTYPE_EXPENSE_GUARDS = 0x6,
			LEDGERITEMTYPE_EXPENSE_CLOTHES = 0x7,
			LEDGERITEMTYPE_EXPENSE_DONCONTROL = 0x8,
			LEDGERITEMTYPE_EXPENSE_FAMILYDEVELOPMENT = 0x9,
			LEDGERITEMTYPE_EXPENSE_OTHER = 0xA,
			LedgerItemType_MAX_VALUE = 0xB,
		};

		class MoneyRecord
		{
		public:

			/**
			 * Reset the Record, all ledgers back to 0.
			 */
			void Reset();

			/**
			 * Get the total amount of income for all ledger types
			 * @return float - Accumulated amount
			 */
			float GetNetIncome() const;

			/**
			 * Get the amount of income from a specific ledger type.
			 * @param LedgerType - Type to query
			 * @return float - Amount of income from queried type
			 */
			float GetItemAmount(const LedgerItemType LedgerType) const;

			/**
			 * Get accumulated total from all ledger types considered as a revenue
			 * @return float - Accumulated revenue from this Money Record.
			 */
			float GetRevenue() const;

			/**
			 * Get accumulated total from all ledger types considered as an expense
			 * @return float - Accumulated expense from this Money Record.
			 */
			float GetExpenses() const;

		private:

			StaticArray<float, LedgerItemType::LedgerItemType_MAX_VALUE> m_LedgerItems;
		};

		class MoneyLedger
		{
		public:
			
		private:

			float m_Balance = 0.0f;
			float m_PeriodicBaseRevenue = 0.0f;
			float m_PeriodicBonusRevenue = 0.0f;
			float m_PeriodicExpenses = 0.0f;
			float m_PeriodicNetIncome = 0.0f;
			EARS::Modules::MoneyRecord m_PeriodicRecord;
			EARS::Modules::MoneyRecord m_TodaysRecord;
			EARS::Modules::MoneyRecord m_YesterdaysRecord;
			EARS::Modules::MoneyRecord m_LifetimeRecord;
		};
	} // Modules
} // EARS
