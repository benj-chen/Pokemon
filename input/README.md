## crit_rates

The chance that a Pokemon lands a critical hit on a pokemon can be divided into stages from [0,4]. Depending on the item that a pokemon is holding (i.e. Razor Claw increases the chance of a critical hit) or the ability of various pokemon, or the nature of some moves (for example, Shadow Claw has a high chance of critical hit according to its move description), the pokemon's chance of getting a crit will become more or less likely. Stage 0 represents an unaffected pokemon using an ordinary move. Stage 1 represents an unaffected pokemon using a move that has a high chance of a critical hit i.e. Shadow Claw. etc.

The way that the input file must be structured:
For lines 1≤i≤5, the ith line represents the chance that a move and Pokemon together at the (i-1)th stage is a critical hit, in percent.

For more information see https://bulbapedia.bulbagarden.net/wiki/Critical_hit

## effect

Depending on the move that a Pokemon uses, its power will also be affected by the defender's type. For example, a water type move would be super effective against a fire type pokemon. Likewise, a fire type move would not be very effective against a water type move. Similarly, an Electric type move actually has no effect at all against a Ground type move.However, I allow the user to change these in the input file. The first line is the multiplier for super effective, the second for not very effective, the third for normal effect, the fourth for no effect, and the fifth for the crit multiplier.

The crit multiplier determines how much to multiply the initial damage by if a critical hit lands.

## type

First, input a number N, representing how many types there are. Prior to generation 6, N=17. For gen 6 and after, N=18 with the addition of the Fairy type. The next N lines determine the names of each type. Then, for each type, input 3 lines. The first of those is a space-delimited list of types that this type is strong to (have an empty line if that type has no types that it's strong to). The second is a space-delimited list of types that this type is weak to. The third is a list of types that it has no effect to. For more information, visit the type chart at https://pokemondb.net/type

## 