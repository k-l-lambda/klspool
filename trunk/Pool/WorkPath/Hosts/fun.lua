
Vector = Billiards.Vector

s_TriangleTop = 6;
s_BallRadius = 0.32 + 1e-4;
s_Sqrt3 = math.sqrt(3.0);


function initialize(game)
	game:loadBallConfigSet"fun"

	local makeBall = Billiards.GameLayout.BallInfo.make
	local balls = {
		makeBall("fun/Main",	Vector(-6, s_BallRadius, 0)),
		makeBall("fun/1#",		Vector(s_TriangleTop, 0, 0) + Vector(0, 1, 0) * s_BallRadius),
		makeBall("fun/2#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3, 1, -1) * s_BallRadius),
		makeBall("fun/3#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3, 1, 1) * s_BallRadius),
		makeBall("fun/4#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 2, 1, -2) * s_BallRadius),
		makeBall("fun/5#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 2, 1, 0) * s_BallRadius),
		makeBall("fun/6#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 2, 1, 2) * s_BallRadius),
		makeBall("fun/7#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 3, 1, -3) * s_BallRadius),
		makeBall("fun/8#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 3, 1, -1) * s_BallRadius),
		makeBall("fun/9#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 3, 1, 1) * s_BallRadius),
		makeBall("fun/10#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 3, 1, 3) * s_BallRadius),
		makeBall("fun/11#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, -4) * s_BallRadius),
		makeBall("fun/12#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, -2) * s_BallRadius),
		makeBall("fun/13#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, 0) * s_BallRadius),
		makeBall("fun/14#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, 2) * s_BallRadius),
		makeBall("fun/15#",		Vector(s_TriangleTop, 0, 0) + Vector(s_Sqrt3 * 4, 1, 4) * s_BallRadius),
	}

	local layout = Billiards.GameLayout()
	local i, ball
	for i, ball in pairs(balls) do
		layout.BallsLayout:push_back(ball)
	end

	game:deployLayout(layout)
end
