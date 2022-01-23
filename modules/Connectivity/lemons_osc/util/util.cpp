namespace lemons::osc
{

bool isValidPort (int port)
{
	return port > 0 && port < 65536;
}

}  // namespace lemons::osc
