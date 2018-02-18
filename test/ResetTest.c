
task main()
{
	float a = -140 * PI / 180;

	float lu = 13;
	float ru = 14;
	float lu2 = lu * lu;
	float ru2 = ru * ru;

	const float lx = 7.5;
	const float ly = 1.8;
	const float lb = atan2(ly, lx);
	const float l2 = lx * lx + ly * ly;
	const float l_ = sqrt(l2);
	const float la = PI / 4.0;
	const float rx = 7.5;
	const float ry = 1.8;
	const float rb = atan2(ry, rx);
	const float r2 = rx * rx + ry * ry;
	const float r_ = sqrt(r2);
	const float ra = PI / 4.0;

	float lq2 = lu * lu + l2 - 2 * lu * l_ * cos(la + PI / 2.0 + lb);
	float rq2 = ru * ru + r2 - 2 * ru * r_ * cos(ra + PI / 2.0 + rb);
	float lq = sqrt(lq2);
	float rq = sqrt(rq2);

	float lc = acos((lq2 + l2 - lu2) / (2.0 * lq * l_));
	float rc = acos((rq2 + r2 - ru2) / (2.0 * rq * r_));

	float lg = PI - a + (PI / 2.0 - lb - lc);
	float rg = a + (PI / 2.0 + rb + rc);

	float y = lq * cos(lg);
	float x = rq * cos(rg);
}
