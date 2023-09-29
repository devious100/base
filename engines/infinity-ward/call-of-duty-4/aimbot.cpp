struct aimbot_ent_t
{
	int iID;
	vec3_t vAimPos;
};
std::vector<aimbot_ent_t>vBestVisible;
std::vector<aimbot_ent_t>vSecondBest;

bool CAimbot::ValidAimTarget(int iID)
{
	entity_t pEntity = cg_entities[iID];

	//if(FriendlyTarget(&clientInfo[iID]) && g_CVar.friends)
	//	return false;
	//if(i==g_CVar.friend1 || i==g_CVar.friend2 || i==g_CVar.friend3)
	//continue;

	if(!gPlayerManager.IsValidEntity(&pEntity))
		return false;

	if(!gPlayerManager.IsEnemy(&pEntity))
		return false;
	return true;
}
int CAimbot::CheckForBetterTarget(int iStart)
{
	for(int i=iStart;i<=cgs->maxPlayers;++i)
	{
		if(!ValidAimTarget(i))
			continue;
	}
}
int CAimbot::RetrieveBestVisibleTargets()
{
	 bool bFoundVis=false;
	 bool bFoundWall=false;

	 vBestVisible.clear();
	 vSecondBest.clear();

	for(int i = 0; i != cgs->maxPlayers; i++) 
	{
		entity_t pEntity = cg_entities[i];
		aimbot_ent_t tmp={0,{0,0,0}};


		if(!ValidAimTarget(i))
			continue;

		// Head point
		gUtils.GetPlayerTag(iHead, &pEntity, tmp.vAimPos);

		// Adjust Aim point
		tmp.vAimPos[2] += 1.75f;

		// Prediction
		AddPrediction(&pEntity, tmp.vAimPos);

		if(gUtils.IsVisible2(tmp.vAimPos, &pEntity)==1)
		{
			tmp.iID=i;
			vBestVisible.push_back(tmp);
			bFoundVis=true;
		}
		if(gUtils.IsVisible2(tmp.vAimPos, &pEntity)==2)
		{
			tmp.iID=i;
			vSecondBest.push_back(tmp);
			bFoundWall=true;
		}
	}

	if(bFoundVis)
		return 1;
	if(bFoundWall)
		return 2;
	return 0;
}
bool CAimbot::GetBestAimTarget(float *flOut)
{
    float flLastDistance = 0xFFFFFF;
    float flCurrentDistance;
    int iClientNum = -1;
	iBestOption=RetrieveBestVisibleTargets();
	int iSize=0;

	if(iBestOption==1)
		iSize=(int)vBestVisible.size();
	if(iBestOption==2)
		iSize=(int)vSecondBest.size();
	else
		return false;

	for(int i = 0; i < iSize; i++) 
	{
		float flDistance=0.0f;
		if(iBestOption==1)
			flDistance=gPlayerManager.GetDistance(&cg_entities[vBestVisible[i].iID]);
		if(iBestOption==2)
			flDistance=gPlayerManager.GetDistance(&cg_entities[vSecondBest[i].iID]);
		
		if((flDistance / 25.0f) <= 30.0f)
		{
			if(iBestOption==1)
				VectorCopy(vBestVisible[i].vAimPos,flOut);
			if(iBestOption==2)
				VectorCopy(vSecondBest[i].vAimPos,flOut);
			return true;
		}
		else 
		{
			flCurrentDistance = flDistance;

			if(flCurrentDistance < flLastDistance) 
			{
				flLastDistance = flCurrentDistance;
				iClientNum = i;
				if(iBestOption==1)
					VectorCopy(vBestVisible[i].vAimPos,flOut);
				if(iBestOption==2)
					VectorCopy(vSecondBest[i].vAimPos,flOut);
			}
		}
	}
	if(iClientNum!=-1)
		return true;
	return false;
}