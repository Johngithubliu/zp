signal void push_key(void)
{
	PORT1|=0x20;
	twatch(0.05);
	PORT1&=~0x20;

}
define button "Test button","pushkey()"
RADIX=10