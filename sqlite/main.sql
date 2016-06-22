/*
workspace
- id : int (pk)
- robots : text -> (x_init, y_init, theta_init, x_goal, y_goal, theta_goal), (x_init, y_init, theta_init, x_goal, y_goal, theta_goal) ...
- obstacles : text -> (x, y, radius, move), (x, y, radius, move)...
- qtd_robots : int
- qtd_obstacles : int

resolution
- id : int (pk)
- date : int (20160606)
- comment : text

statistics
- id : int (pk)
- id_workspace : int (fk)
- id_resolution : int (fk)
- success : bool
- qtd_collisions : int
- qtd_steps : int 
*/

CREATE TABLE `workspace` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	`robots`	TEXT NOT NULL,
	`obstacles`	TEXT,
	`qtd_robots`	INTEGER NOT NULL,
	`qtd_obstacles`	INTEGER NOT NULL
);

CREATE TABLE `resolution` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	`date`	INTEGER NOT NULL,
	`comment`	TEXT NOT NULL
);

CREATE TABLE `statistics` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	`id_workspace`	INTEGER NOT NULL,
	`id_resolution`	INTEGER NOT NULL,
	`success`	INTEGER NOT NULL,
	`qtd_collisions`	INTEGER NOT NULL,
	`qtd_steps`	INTEGER NOT NULL
);

