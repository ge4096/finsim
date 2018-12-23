#pragma once

#include "account/account.h"
#include "account/checking_account.h"
#include "test/unit_test.h"

class CheckingAccountTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(CheckingAccountTest);
		RUN_TEST_CASE(initializeZero);
		RUN_TEST_CASE(initializePositive);
		RUN_TEST_CASE(initializeNegative);
		RUN_TEST_CASE(deposit);
		RUN_TEST_CASE(withdraw);
		RUN_TEST_CASE(dailyUpdate);
		RUN_TEST_CASE(transfer);
		END_TEST(CheckingAccountTest);
	}

private:
	void initializeZero() {
		Dollars const zeroBalance = Dollars();
		std::string const accountName = "My Account";
		
		CheckingAccount account(accountName, zeroBalance);
		ASSERT_EQUAL(account.getValue(), zeroBalance);
		ASSERT_EQUAL(account.getName(), accountName);
	}
	
	void initializePositive() {
		Dollars const positiveBalance = Dollars(12, 34);
		std::string const accountName = "My Positive Account";
		
		CheckingAccount account(accountName, positiveBalance);
		ASSERT_EQUAL(account.getValue(), positiveBalance);
		ASSERT_EQUAL(account.getName(), accountName);
	}
	
	void initializeNegative() {
		Dollars const negativeBalance = Dollars(-12, -34);
		std::string const accountName = "My Negative Account";
		
		ASSERT_THROW(CheckingAccount account(accountName, negativeBalance),
		              Account::Overdraw);
	}
	
	void deposit() {
		Dollars const startingBalance = Dollars(12, 34);
		Dollars const depositAmount = Dollars(1, 23);
		
		CheckingAccount account("My Account", startingBalance);
		account.deposit(Dollars());
		ASSERT_EQUAL(account.getValue(), startingBalance);
		account.deposit(depositAmount);
		ASSERT_EQUAL(account.getValue(), startingBalance + depositAmount);
		account.deposit(Dollars());
		ASSERT_EQUAL(account.getValue(), startingBalance + depositAmount);
	}
	
	void withdraw() {
		Dollars const startingBalance = Dollars(12, 34);
		Dollars const withdrawAmount = Dollars(1, 23);
		
		CheckingAccount account("My Account", startingBalance);
		account.withdraw(Dollars());
		ASSERT_EQUAL(account.getValue(), startingBalance);
		account.withdraw(withdrawAmount);
		ASSERT_EQUAL(account.getValue(), startingBalance - withdrawAmount);
		account.withdraw(Dollars());
		ASSERT_EQUAL(account.getValue(), startingBalance - withdrawAmount);
		ASSERT_THROW(account.withdraw(startingBalance), Account::Overdraw);
	}
	
	void dailyUpdate() {
		Dollars const startingBalance = Dollars(1234567, 89);
		AccountUpdate update(Date(2008, 10, 13));
		update.dailyMarketChange = 111.58;
		update.annualInflation = 0.9964;
		
		CheckingAccount account("My Account", startingBalance);
		account.dailyUpdate(update);
		ASSERT_EQUAL(account.getValue(), startingBalance);
	}
	
	void transfer() {
		Dollars const startingBalance = Dollars(12, 34);
		Dollars const transferAmount = Dollars(1, 23);
		CheckingAccount account1("Account 1", startingBalance);
		CheckingAccount account2("Account 2", startingBalance);
		
		account1.transferTo(&account2, transferAmount);
		ASSERT_EQUAL(account1.getValue(), startingBalance - transferAmount);
		ASSERT_EQUAL(account2.getValue(), startingBalance + transferAmount);
		
		account1.transferFrom(&account2, transferAmount);
		ASSERT_EQUAL(account1.getValue(), startingBalance);
		ASSERT_EQUAL(account2.getValue(), startingBalance);
		
		account1.transferTo(&account2, startingBalance);
		ASSERT_EQUAL(account1.getValue(), Dollars());
		ASSERT_EQUAL(account2.getValue(), 2 * startingBalance);
		
		ASSERT_THROW(account1.transferTo(&account2, transferAmount),
		             Account::Overdraw);
	}

};
