##Isabel Asconape
##Welcome to my budget tracker!!
import json
import os

##How to Use This Project:
##	1.	Add Income or Expense: Enter the amount and category when prompted.
##	2.	View Summary: Get a summary of total income, expenses, and balance, along with category breakdowns.
##	3.	Data Persistence: Your transactions are saved in a JSON file (budget_data.json), so they persist between runs.
##What This Shows:
##	•	Object-Oriented Programming: Use of classes to encapsulate functionality.
##	•	File Handling: Saving and loading data from a file.
##	•	Data Management: Summarizing and categorizing data.

class BudgetTracker:
    def __init__(self, filename='budget_data.json'):
        self.filename = filename
        self.transactions = self.load_data()

    def load_data(self):
        if os.path.exists(self.filename):
            with open(self.filename, 'r') as file:
                return json.load(file)
        return {'income': [], 'expenses': []}

    def save_data(self):
        with open(self.filename, 'w') as file:
            json.dump(self.transactions, file, indent=4)

    def add_transaction(self, amount, category, transaction_type):
        if transaction_type not in ['income', 'expenses']:
            raise ValueError("Transaction type must be 'income' or 'expenses'")
        
        self.transactions[transaction_type].append({
            'amount': amount,
            'category': category
        })
        self.save_data()

    def view_summary(self):
        total_income = sum(t['amount'] for t in self.transactions['income'])
        total_expenses = sum(t['amount'] for t in self.transactions['expenses'])
        balance = total_income - total_expenses

        print("\n--- Budget Summary ---")
        print(f"Total Income: ${total_income:.2f}")
        print(f"Total Expenses: ${total_expenses:.2f}")
        print(f"Balance: ${balance:.2f}")

        print("\nIncome Categories:")
        self.print_category_summary('income')

        print("\nExpense Categories:")
        self.print_category_summary('expenses')

    def print_category_summary(self, transaction_type):
        categories = {}
        for transaction in self.transactions[transaction_type]:
            category = transaction['category']
            amount = transaction['amount']
            if category in categories:
                categories[category] += amount
            else:
                categories[category] = amount
        
        for category, amount in categories.items():
            print(f"{category}: ${amount:.2f}")

def main():
    tracker = BudgetTracker()

    while True:
        print("\n--- Budget Tracker Menu ---")
        print("1. Add Income")
        print("2. Add Expense")
        print("3. View Summary")
        print("4. Exit")

        choice = input("Select an option: ")

        if choice == '1':
            amount = float(input("Enter income amount: "))
            category = input("Enter income category: ('income' or 'expences')")
            tracker.add_transaction(amount, category, 'income')
        elif choice == '2':
            amount = float(input("Enter expense amount: "))
            category = input("Enter expense category: ")
            tracker.add_transaction(amount, category, 'expenses')
        elif choice == '3':
            tracker.view_summary()
        elif choice == '4':
            print("Exiting Budget Tracker.")
            break
        else:
            print("Invalid option, please try again.")

if __name__ == '__main__':
    main()

