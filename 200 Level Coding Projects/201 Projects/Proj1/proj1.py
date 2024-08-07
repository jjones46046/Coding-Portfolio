"""
File:    proj1.py
Author:  Jared Jones
Date:    10/18/2019
Section: 33
E-mail:  jjones20@umbc.edu
Description:
  Baking Simulator
"""

STARTING_FUNDS = 80.0
USER_SPENT = 0
FLOUR_COST_PER_BAG = 2.5
EGGS_COST_PER_CRATE = 1.9
YEAST_COST_PER_BAG = 3.25
BUTTER_COST_PER_PACK = 1.25
MILK_COST_PER_JUG = 2.75
GALLONS_MILK_PER_JUG = 1.0
EGGS_PER_CRATE = 24.0
TBSP_BUTTER_PER_PACK = 64.0
CUPS_FLOUR_PER_BAG = 25.0
CUP_YEAST_PER_BAG = 6.0
CROISSANT_FLOUR = 48.0
BAGUETTE_FLOUR = 112.5
CROISSANT_EGGS = 12.0
MACARON_EGGS = 60
EGGS_REQUIRED = 72.0
FLOUR_REQUIRED = 160.5
BUTTER_REQUIRED = 144.0
YEAST_REQUIRED = 89.0
CROISSANT_YEAST = 39
BAGUETTE_YEAST = 50
MILK_REQUIRED = 18.0
CUPS_IN_GALLON = 16
TSP_IN_CUP = 48

if __name__ == "__main__":
    item_prices = [FLOUR_COST_PER_BAG,'flour',EGGS_COST_PER_CRATE,'eggs',YEAST_COST_PER_BAG,'yeast',BUTTER_COST_PER_PACK,'butter',MILK_COST_PER_JUG,'milk']
    shopping_list = ['yeast','eggs','milk','flour','butter']
    ingredients = ['yeast','tsp',CUP_YEAST_PER_BAG,YEAST_REQUIRED,'eggs','eggs',EGGS_PER_CRATE,EGGS_REQUIRED,'milk','cups',CUPS_IN_GALLON,MILK_REQUIRED,'flour','cups',CUPS_FLOUR_PER_BAG,FLOUR_REQUIRED,'butter','tbsp',TBSP_BUTTER_PER_PACK,BUTTER_REQUIRED]
    tasks = ['Begin the baguette dough','Make the macarons','Make the cr\
oissants','Bake the baguettes']
    remaining_tasks = tasks
    
    def opening_remarks():
        """ 
Prints the static, opening remarks giving the user a list of ingredients
and their ammounts, that are needed for all three foods.
        """

        print('Baker!  You must buy the following with $',int(STARTING_FUNDS))
        print()
        print('For 300 croissants:')
        print('  ',int(CROISSANT_FLOUR),'cups of flour')
        print('  ',int(CROISSANT_EGGS),'eggs')
        print('  ',int(BUTTER_REQUIRED),'tbsp of butter')
        print('  ',int(CROISSANT_YEAST),'tsp of yeast')
        print('  ',int(MILK_REQUIRED),'cups of milk')
        print()
        print('For 600 macarons:')
        print('  ',int(MACARON_EGGS),'eggs')
        print()
        print('For 100 baguettes:')
        print('  ',int(BAGUETTE_FLOUR),'cups of flour')
        print('  ',int(BAGUETTE_YEAST),'tsp of yeast')

        
    def budget_tracker(item,amount):
        """
      Tracks how much the user has spent after each purchase of ingredients.

        """
        budget_check = False
        global STARTING_FUNDS
        global USER_SPENT
        if item in shopping_list:
            item_index = (item_prices.index(item))
            items_cost = ((amount)*(item_prices[item_index - 1]))
            USER_SPENT = USER_SPENT + items_cost
        else:
            budget_check = True
        if USER_SPENT > STARTING_FUNDS:
            budget_check = False
        elif USER_SPENT <= STARTING_FUNDS:
            budget_check = True
            
        return budget_check

    
    def go_shopping():
        """
Asks the user for what type of ingredient they want to purchase, then ask 
for the amount they would like to purchase. Can be stopped if the user
enters "NOTHING" or simply runs over budget.
        """

        global USER_SPENT
        user_item = ''
        amount = 0
        budget_check = True
        counter = 0
        while user_item != 'NOTHING' and counter != 5:
            budget_check = budget_tracker(user_item,amount)
            if budget_check == True:
                user_item = str(input("What would you like to purchase? (ENTER 'NOTHING' to leave.) \n"))
                if user_item == 'eggs':
                    amount = int(input('How many crates of eggs would you like? '))
                    counter += 1
                    for i in range(amount):
                        shopping_list.append(user_item)
                elif user_item == 'flour' or user_item == 'yeast':
                    print('How many bags of '+user_item+' would you like?',end =' ')
                    amount = int(input())
                    counter += 1
                    for i in range(amount):
                        shopping_list.append(user_item)
                elif user_item == 'butter':
                    amount = int(input('How many packs of butter would you like to purchase? '))
                    counter += 1
                    for i in range(amount):
                        shopping_list.append('butter')
                elif user_item == 'milk':
                    amount = int(input('How many jugs of milk would you like? '))
                    counter += 1
                    for i in range(amount):
                        shopping_list.append('milk')
                elif user_item == 'NOTHING':
                    pass
                else:
                    print('That is not a valid entry.')
#Gives the user an item-specific question of how much of the item they want
            elif budget_check == False:
                print('You have run out of money!')
                user_item = 'NOTHING'
                
        return shopping_list
        
    def check_items_enough(shopping_cart,ingredients):
        """
        Checks the user's shopping list to see if they have enough ingredients to bake successfully. Fails them if they do not
        """
        
        check_good = 0
        for food in ingredients[0:len(ingredients):4]:
            if food == 'yeast':
                user_amount = (shopping_cart.count('yeast')-1)*(CUP_YEAST_PER_BAG)*TSP_IN_CUP
                if user_amount < YEAST_REQUIRED:
                    print("You didn't buy enough yeast! We needed:",(YEAST_REQUIRED-user_amount),'more tsp')
                else:
                    check_good += 1
#Checks the amount of yeast the user bought, and compares it with the amount we need to bake with
            elif food in shopping_cart and food != 'yeast':
                item_index = ingredients.index(food)
                requirement = ingredients[item_index + 3]
                unit_of_measurement = ingredients[item_index + 1]
                user_amount = (shopping_cart.count(food)-1)*ingredients[(item_index+2)]
#item_index, is the food the program is currently using,
#requirement is the amount of that food is needed, unit_of_measurement is the unit of measurement for that particular food,
#and user_amount is a calculated value of how much of the ingredient the user has.
                if user_amount < requirement:
                    print("You didn't buy enough",food,"!",'We needed:',(requirement - user_amount),'more',unit_of_measurement)
                    check_good = 0

                else:
                    check_good += 1
#check_good works like a checklsit, each ingredient for which we have a sufficent amount,
#is one check. If we end up with 5 checks, we can bake.
                    
        if check_good == 5:
            print('We have everything we need! Ready! Set! Bake!')
        else:
            print('You did not buy enough ingredients to even begin baking. You are a failure of all sorts. Good day.')
            
        return 5

    def is_valid_input(index, tasks):
        if len(tasks) == 4:
            options = 0
            bad_options = range(1,4)
        elif tasks(0) not in tasks:
            options = range(0,2)
            bad_options = 2
        elif len(tasks) == 1:
            options = 0
            
        if index in options:
            input_check = True
        elif index in bad_options:
            input_check = False
            
    def bake():
        tasks = ['Begin the baguette dough','Make the macarons','Make the croissants','Bake the baguettes']
        remaining_tasks = tasks
        remaining_indexs = [0,1,2,3]
        counter = 1
        
        while remaining_tasks != 0:
            print('Here are your remaining tasks of the day')
            for task in remaining_tasks:
                print(counter,'- ',task)
                counter += 1
            index = int(input('What action would you like to cross off your list next?'))
            if is_valid_input(index,remaining_tasks) == True:
                remaining_tasks.append(index)
            else:
                print("Yeah that's not gonna work, lets try again")
        

        
        
        
    opening_remarks()
    print('\n', end = '\n')
    go_shopping()
    check_items_enough(shopping_list,ingredients)
    if check_items_enough(shopping_list,ingredients) == 5:
        bake()
