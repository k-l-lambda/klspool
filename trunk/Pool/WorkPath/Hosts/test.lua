
Vector = Billiards.Vector

s_TriangleTop = 6;
s_BallRadius = 0.32 + 1e-4;
s_Sqrt3 = math.sqrt(3.0);


function initialize(game)
	local layout = Billiards.GameLayout()

	--[[local ball1 = Billiards.GameLayout.BallInfo.make("std/Main", Vector(-6, s_BallRadius, 0))
	layout.BallsLayout:push_back(ball1)

	local ball2 = Billiards.GameLayout.BallInfo.make("std/1#", Vector(s_TriangleTop, 0, 0) + Vector(0, 1, 0) * s_BallRadius)
	layout.BallsLayout:push_back(ball2)]]
	local makeBall = Billiards.GameLayout.BallInfo.make
	local balls = {
		makeBall("std/Main",	Vector(-6, s_BallRadius, 0)),
		makeBall("std/1#",		Vector(s_TriangleTop, 0, 0) + Vector(0, 1, 0) * s_BallRadius),
		makeBall("std/2#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3, 1, -1) * s_BallRadius),
		makeBall("std/3#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3, 1, 1) * s_BallRadius),
		makeBall("std/4#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 2, 1, -2) * s_BallRadius),
		makeBall("std/5#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 2, 1, 0) * s_BallRadius),
		makeBall("std/6#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 2, 1, 2) * s_BallRadius),
		makeBall("std/7#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 3, 1, -3) * s_BallRadius),
		makeBall("std/8#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 3, 1, -1) * s_BallRadius),
		makeBall("std/9#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 3, 1, 1) * s_BallRadius),
		makeBall("std/10#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 3, 1, 3) * s_BallRadius),
		makeBall("std/11#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, -4) * s_BallRadius),
		makeBall("std/12#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, -2) * s_BallRadius),
		makeBall("std/13#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, 0) * s_BallRadius),
		makeBall("std/14#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, 2) * s_BallRadius),
		makeBall("std/15#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, 4) * s_BallRadius),
	}

	local i, ball
	for i, ball in pairs(balls) do
		layout.BallsLayout:push_back(ball)
	end

	game:deployLayout(layout)
end
