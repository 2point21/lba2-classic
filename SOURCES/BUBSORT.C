void BubbleSort( UBYTE	*base,
		size_t	nelem,
		size_t	width,
		int (*fcmp)(const void *, const void *))
{
	int	i, j;
	register UBYTE	*o1, *o2, *s;
	UBYTE	*temp=&base[width*nelem];

	o1 = base ;
	for(i=1; i<nelem; i++)
	{
		o2 = o1+width ;
		s = o1 ;
		for(j=i; j<nelem; j++)
		{
			if ((*fcmp)(s, o2)>0)
				s = o2 ;
			o2 += width;
		}
		if (s!=o1)
		{
			memcpy(temp, o1, width);
			memcpy(o1,    s, width);
			memcpy(s,  temp, width);
		}
		o1 += width;
	}
}