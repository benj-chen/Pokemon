# run this from the same directory that bot.py is in
import os

dexn = int(input("pokedex number, leading zeroes not necessary: "))

gendn = int(input("gender variations? 1 or 2 "))

formn = int(input("extra form count? i.e. shellos is 2 because no default form, starly is 0 "))

form_names = [input("enter form #{}".format(i)) for i in range(1,formn+1)]

