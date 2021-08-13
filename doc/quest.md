# Classes

## `Quest`
The top-level object in the heirarchy that is most user-facing. Quest is mostly just a container for the Activation and Tasks. A Quest has different possible statuses:
  * `:new` when a Quest is cloned from the Zone and made available to the player, but has not been accepted yet.
  * `:active` once the Quest has been accepted by the player but has not yet been completed nor has it failed.
  * `:complete` when all Tasks have become `:complete`
  * `:failed` when any Task has become `:failed`

## `Task`
Task is also somewhat of a container object also, but itself encapsulates an entire completable chore. Tasks also have different possible statuses sharing a lot of similarity to Quest statues. Task statuses are:
  * `:idle` when the Quest is active but not all parent Tasks have been completed.
  * `:active` once all parent Tasks have been completed.
  * `:complete` once the Objective has been met while none of the Parameters were failed.
  * `:failed` when any of the Parameters were failed.

Tasks are organized as a multiroot DAG inside of a Quest. That is to say that any Task may have zero or more parents and zero or more children. Each Task is stored as a member of a flat Array in the Quest, and each Task maintains its own parent & child links. This makes serialization and deserialization easier (allowing multiple parents presents difficulties) and doesn't hurt runtime performance much since each Task is tightly coupled with the Quest anyway.

A few notes on Tasks and how they are organized under Quests:
  1. A Task with no parents is considered a "root" Task, and becomes active when the Quest is accepted.
  2. A Task with one or more parents may not become active until ALL of its parent Tasks are completed.
  3. When a Task is completed all of its children are evaluated and, iff they satisfy point 2 above, become active.
  4. A Quest is not complete until and unless all Tasks are complete.

A Task may be flagged as `nospoil`. When a Task is nospoil, it is not shown to the player until it becomes `:active`. This - you may not have been clever enough to figure out thanks to the creative name - is to avoid potential spoilers. Only non-root Tasks may be nospoil, and each Task must be nospoiled individually during configuration (i.e. nospoiling one Task does not automatically mark all of its children nospoil).

## `Spy`
Spy is an underlying abstraction that powers both Objectives and Parameters because they share a common pattern of hooking and signaling other arbitrary objects. Spy objects will dynamically call methods (if defined) with set prefixes and suffixes based on the type of underlying trigger (e.g. `setup_visit_room` or `exec_kill_npc`).

## `Objective`
A Task will have one Objective. Visit X room, Kill Y NPC, talk to Z shopkeep, etc. Different kinds of objective will have their own `exec_*` method and an optional `setup_*` method also. The Task is considered complete if the Obective has been met (the `exec_*` method has returned a value of `:complete`) and none of the Parameters `exec_*` methods have returned `:fail`. Typically an Objectives' `exec_*` will return `:complete` once some condition is met. For example, the `:visit_room` objective fires its `exec_*` method (specifically `exec_visit_room`) every time the Being enters a new Room. It checks the RID of the Room and returns `:complete` if the RID matches its `@params`.

## `Parameter`
A Parameter - like an Objective - has different forms. `setup_*`, `activate_*`, and `deactivate_*` methods are optionally called, and `exec_*` will typically return `:fail` if its condition is met. For example, a Parameter may be that you can't enter combat while you attempt your Objective. In that case, the `exec_no_fighting` method would be invoked when a Being enters Combat, and would immediately return `:fail`.

## `Reward`
A Reward object encapsulates the proximal incentive for completing the Task. When the Task is completed, the hero receives whatever is configured in the Reward (for example, gold, items, or experience).

## `Activation`
Activations are a bit different than the others. Every Quest has exactly one Activation, which is the condition under which the Quest is made available to the player. While a Spy will typically monitor the activity of a given Being, an Activation will monitor activity of the rest of the world, watching for events triggered by Beings. For example, if you are granted the Quest upon entering a certain Room, the Activation needs to monitor for new Beings being added to that Room. Using the same kind of pattern found in Spys, an Activation will execute a `test_*` method where you can add logic e.g. a level requirement.