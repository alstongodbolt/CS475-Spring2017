kernel
void
ArrayMult( global const float *dA, global const float *dB, global float *dC )
{
	int gid = get_global_id( 0 );
	int numItems = get_local_size(0);
	int ynum = get_local_id(0);
	int wgNum = get_group_id(0);

	prods[tnum] = dA[gid] * dB[gid]
}
