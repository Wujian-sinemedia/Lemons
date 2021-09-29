
namespace lemons::plugin
{
void Preset::serialize (TreeReflector& ref)
{
    ref.add ("State", state);
    ref.add ("Name", name);
    ref.add ("Category", category);
    ref.add ("Author", author);
    ref.add ("Description", description);
    ref.add ("Version", version);
}

}  // namespace lemons::plugin
