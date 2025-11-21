class OHSNAP_API FOhSnapCallbacks
{
public:
	static void SnapActorToActor(bool bTranslation = true, bool bRotation = true, bool bReverse = false);
	static bool SnapActorToActor_CanExecute();
};