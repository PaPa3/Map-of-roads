Program that simulates map of roads. The map is represented as undirected graph. The map consists of
cities (vertices), roads (edges with two attributes: length, built year) and national route (simple paths the in graph).
The goal of the program is to keep national route as short as possible while roads (edges) may change (edge can be
deleted/added or built year may change).

## Installation

```bash
cmake .
make
```

## Usage

```
routeId;cityName;length;builtYear;cityName;...;cityName
```
Adds new national route with routeId which through given cites and roads. If given cites or roads does not exits
then they are added to the map.

```
addRoad;city1;city2;length;builtYear
```
Adds given road to the map

```
repairRoad;city1;city2;repairYear
```
Changes built year of given road.

```
getRouteDescription;routeId
```
Prints a description of national route routeId

```
newRoute;routeId;city1;city2
```
Creates new national route from city1 to city2. Choses the shortest possible national route among all valid routes.
If there are multiply answer then choses the route which the oldest is as young as possible.

```
extendRoute;routeId;city
```
Extends national route routeId to the given city.

```
removeRoad;city1;city2
```
Removes road from the map. If some route becomes disconnected then makes a detour for given national route.

```
removeRoute;routeId
```
Removes national route routeId from the map.
