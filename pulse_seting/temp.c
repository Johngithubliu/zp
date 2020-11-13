void forward()
{
	unsigned char i,n,n_high;
	unsigned int pulse_least;

	unsigned int pulse_h;
	unsigned char mult_pulse;
	unsigned int pulse_remainder;
	
	if(max_speed>2500)max_speed=2500;
	if(max_speed<80)max_speed=80;
	n_high=255-2500*8/max_speed;
	
	if(direct1)DIRECT=CW;
	else DIRECT=CCW;
		
	if(pulse_l<real_up*2)
	{
		mult_pulse=0;
		pulse_remainder=0;
		pulse_h=pulse_l/2;
		top=0;
	}
	else
	{
		//top=pulse_l-real_up-real_up;
		mult_pulse=(pulse_l-real_up-real_up)/(unsigned int)0xffff;
		pulse_remainder=(pulse_l-real_up-real_up)%(unsigned int)0xffff;
		pulse_h=real_up;
	}
	
	
	pulse=pulse_h;
	
	n=147;
	TH0=n;
	TR0=1;
		
	while(pulse)
	{
		if(n<n_high)n++;
	
		TH0=n;
		delay01ms();
		READP0;
		if(TOC1)
		{TR0=0;pulse=0;return;};	
	} //wait...;


//-----------top segment----------------			
		//  TEST=1; 	
//-------------high speed-------------------------

		if(pulse_remainder)pulse=pulse_remainder;
		TR0=1;
			while(pulse)
		{	
			READP0;
			if(TOC1)
			{TR0=0;break; };
		}
		
//--- remainder ok---		
		
		for(i=0;i<mult_pulse;i++)
		{
			pulse=(unsigned int)0xffff;
			TR0=1;
			while(pulse)
			{
				READP0;
				if(TOC1)
				{TR0=0;pulse=0;return;};	
			} //wait...;
		
		}
//----------above is top----------------	

//-------------high speed end------------------
	
		pulse=pulse_h;
	
	n=n_high;
	TH0=n;
	TR0=1;
		
	while(pulse)
	{
		if(n>220)n--;
	
		TH0=n;
		delay1ms();
	
		READP0;
		if(TOC1)
		{TR0=0;pulse=0;return;};	
	} //wait...;



		while(pulse)
		{	
			READP0;
			if(TOC1)
			{TR0=0;break; };
		}
		
		real_up=pulse_h-pulse_least;
		up=real_up;
		down=up;
		top=pulse_remainder;
		if(f_first)
		{		
			f_first=0;f_save=1;
		}
		//debug=mult_pulse;
		time_used=(pulse_l-real_up-real_up)*0.021+320;

}


