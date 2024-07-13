#include "MoneyLedger.h"

// addons
#include "Addons/Hook.h"

void EARS::Modules::MoneyRecord::Reset()
{
	for (uint32_t i = 0; i < m_LedgerItems.GetCapacity(); i++)
	{
		m_LedgerItems[i] = 0.0f;
	}
}

float EARS::Modules::MoneyRecord::GetNetIncome() const
{
	float OutAmount = 0.0f;

	for (uint32_t i = 0; i < m_LedgerItems.GetCapacity(); i++)
	{
		OutAmount += m_LedgerItems[i];
	}

	return OutAmount;
}

float EARS::Modules::MoneyRecord::GetItemAmount(const LedgerItemType LedgerType) const
{
	// TODO: Check whether it is within bounds

	return m_LedgerItems[LedgerType];
}

float EARS::Modules::MoneyRecord::GetRevenue() const
{
	float OutAmount = 0.0f;

	for (uint32_t i = LedgerItemType::LEDGERITEMTYPE_REVENUE_RACKETS; i < LedgerItemType::LEDGERITEMTYPE_REVENUE_OTHER; i++)
	{
		OutAmount += m_LedgerItems[i];
	}

	return OutAmount;
}

float EARS::Modules::MoneyRecord::GetExpenses() const
{
	float OutAmount = 0.0f;

	for (uint32_t i = LedgerItemType::LEDGERITEMTYPE_EXPENSE_BRIBES; i < LedgerItemType::LEDGERITEMTYPE_EXPENSE_OTHER; i++)
	{
		OutAmount += m_LedgerItems[i];
	}

	return OutAmount;
}
