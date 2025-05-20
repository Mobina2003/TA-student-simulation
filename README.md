# TA student simulation
This problem describes a simulation of how a Teaching Assistant (TA) interacts with students in a small office. The TA provides guidance to students studying operating systems concepts but can only help one student at a time. Meanwhile, up to five students can wait in the office, and if the seats are full, new students must leave and return later.


## Key System Rules:
* 1.The TA can only assist one student at a time.
* 2.At most, five students can wait in the office.
* 3.If no seats are available, a student must leave and try again later.
* 4.If the TA is sleeping, the first arriving student wakes them up.
* 5.After helping a student, the TA checks whether there are other waiting students.
* 6.If yes, the TA assists the next student.
+ 7.If no, the TA goes back to sleep
 
