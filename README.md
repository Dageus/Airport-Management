# Airport-Management
Script that allows the creation of airports, flights, and reservations, and management of each one.

One can create an airport, with the 'a' command, which is identified by a 3-digit code, and includes the city where it is and the country;
the limit of airports is 40.

Flights can also be created, with the use of the 'v' command, and include a 7-digit code, and the airport of origin and the airport of destination, the time of the flight, the number of passagers, and the duration;
the limit of flights is 30 000.

Reservations for a flight can be made with the 'r' command, and are mde up of an at least 10-digit code.

Both Flights and Reservations can be eliminated with the 'e' command.

The `l` command will list every airport in alphabetical order.

If the `v` command is called with no arguments, it will list every flight in order of creation.

The `t` command changes the date of the system.

The `p` command lists every flight with origin in the airport which the command receives, with the hour of parture.

The `c` command lists every flight with destination in the airport which the command receives, with the hour of arrival.
