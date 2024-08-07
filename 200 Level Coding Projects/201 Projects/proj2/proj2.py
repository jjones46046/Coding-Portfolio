"""
File:    proj2.py
Author:  Jared Jones
Date:    11/8/2019
Section: 33
E-mail:  jjones20@umbc.edu
Description:
  Sim Game
"""
from proj2_ui import print_board

p1_character = ''
p2_character = ''
p1_lines = []
p2_lines = []
winner = False
if __name__ == '__main__':
        def symbols(num_player):
                Pass = False
        while Pass == False:
            print('Player',num_player,'What symbol would you like to use?')
                character = str(input())
                if len(character) == 1:
                        Pass = True
                else:
                        Pass = False
                return character
        
        def converter(ply_move):
                line = []
                ply_move = ply_move.split()
                for cord in ply_move:
                        new_cord = int(cord)
                        line.append(new_cord)
                        return line
        def storage_manager(num_player):
                is_valid = False
                while is_valid == False:
                        line = []
                        print('Enter a line for player',num_player,': ');ply_move = input()
                        line = converter(ply_move)
                        if len(line) == 2:
                                is_valid = input_valid(line, p1_lines, p2_lines)
                                #Tests the player move to see if its valid.
                                if is_valid == False:
                                        print('That was invalid please try again')
                                
                                if is_valid == True:
                                        if num_player == 1:
                                                p1_lines.append(line)
                                                if num_player == 2:
                                                        p2_lines.append(line)
                #Adds the valid line to the appropriate move storage
                
                return p1_lines, p2_lines
        
        def input_valid(line,p1_lines,p2_lines):
                is_valid = True
                current_lines = p1_lines + p2_lines
                alt_line = list(line)
                alt_line.reverse()

                for num in line:
                        if not 1 <= num <= 6:
                                is_valid = False
                                #check to see if each vertice is between 1 and 6
                                if (line[0] - line[1]) == 0:
                                        #checks to see if the line is drawing from the same point
                                        is_valid = False
                                        if line in current_lines or alt_line in current_lines:
                                                #checks to see if the line has already been draw
                                                is_valid = False
                                                
                                                return is_valid
                                        
                                        def victory(p1_lines):
                                                lenghtP1 = len(p1_lines)
                                                itr = 0
                                                first_num = 0
                                                innerCounter = 0
                                                outerCounter = 0
                                                num_occurence = 0
                                                
                                                
                                                neighbor_array_1 = []
                                                neighbor_array_2 = []
                
                                                #goes through 2d array, if a a num occurs more than once, adds its neighbors to an array
                                                while itr <= 1:
                                                        first_num = p1_lines[lenghtP1-1][innerCounter]
                                                        while outerCounter < lenghtP1 and num_occurence < 2:
                                                                if first_num == p1_lines[outerCounter][0]:
                                                                        neighbor_array_1.append(p1_lines[outerCounter][1])
                                                                        num_occurence += 1
                                                                        if first_num == p1_lines[outerCounter][1]:
                                                                                neighbor_array_1.append(p1_lines[outerCounter][0])
                                                                                num_occurence += 1
                                                                                outerCounter += 1
                                                
                                                                                outerCounter = 0
                                                                                innerCounter = 0
                                                                                itr += 1
                                                                                #goes through first array, gets the neighbirs of each value
                                                                                for l in range(len(neighbor_array_1)):
                                if num_occurence > 1:
                                        #iadds the neighbors ofthe second arrau
                                        for s in range(len(p1_lines)):
                                                if neighbor_array_1[l] == p1_lines[s][0]:
                                                        neighbor_array_2.append(p1_lines[s][1])
                                                        if neighbor_array_1[l] == p1_lines[s][1]:
                                                                neighbor_array_2.append(p1_lines[s][0])
                                                                #goes thorugh the nieghbors of second array and checks is they have first num as a neighbor
                                                                for z in range(len(neighbor_array_2)):
                                                                        for h in range(len(p1_lines)):
                                                                                if neighbor_array_2[z] == p1_lines[h][0]:
                                                                                        if p1_lines[h][1] == first_num:
                                                                                                return True
                                                                if neighbor_array_2[z] == p1_lines[h][1]:
                                                                        if p1_lines[h][0] == first_num:
                                                                                #there is a triangle
                                                                                return True
                                                                        
                                                                        
                                                                        return False
                                                                
                                                                def turn_manager(num_player):
                                                                        print_board(p1_lines,p1_character,p2_lines,p2_character)
                                                                        storage_manager(num_player)
                                                                        winner = victory(p1_lines)
                                                                        if num_player == 1:
                                                                                winner = victory(p1_lines)
                                                                                if winner == True:
                                                                                        print('Game over, Player 1 loses!')
                                                                                        if num_player == 2:
                        winner = victory(p2_lines)
                        if winner == True:
                                print('Game over, Player 2 loses!')
                                
                                return winner
                        
                        p1_character = symbols(1)
                        p2_character = symbols(2)
                        
                        
        while winner == False:
                winner = turn_manager(1)
                if winner == False:
                        winner = turn_manager(2)
                        
