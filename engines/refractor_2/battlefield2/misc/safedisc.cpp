

#include "safedisc.h"


SafediscFix::SafediscFix(): applied(FALSE)
{
	CodePatch patch;

	patch.location = CodePatch::on_bf2;

	//fix 0
	patch.offset = 0x000015D4;
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x0D);
	patch.patched_code.push_back(0x38);
	patch.patched_code.push_back(0x1D);
	patch.patched_code.push_back(0x96);
	patch.patched_code.push_back(0x00);

	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x11);
	patch.patched_code.push_back(0x8D);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xFB);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 1
	patch.offset = 0x000016DC;
	patch.patched_code.push_back(0x74);
	patch.patched_code.push_back(0x34);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 2
	patch.offset = 0x00001734;
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x0D);
	patch.patched_code.push_back(0x38);
	patch.patched_code.push_back(0x1D);
	patch.patched_code.push_back(0x96);
	patch.patched_code.push_back(0x00);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x11);
	patch.patched_code.push_back(0x8D);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xFD);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 3
	patch.offset = 0x000017F1;
	patch.patched_code.push_back(0x33);
	patch.patched_code.push_back(0xC0);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 4
	patch.offset = 0x0000181C;
	patch.patched_code.push_back(0x8b);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x0D);
	patch.patched_code.push_back(0x38);
	patch.patched_code.push_back(0x1D);
	patch.patched_code.push_back(0x96);
	patch.patched_code.push_back(0x00);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x11);
	patch.patched_code.push_back(0x8D);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xFF);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 5
	patch.offset = 0x000018B6;
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x0D);
	patch.patched_code.push_back(0x38);
	patch.patched_code.push_back(0x1D);
	patch.patched_code.push_back(0x96);
	patch.patched_code.push_back(0x00);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x11);
	patch.patched_code.push_back(0x8D);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xFE);
	patch.patched_code.push_back(0x50);
	patch.patched_code.push_back(0x8D);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xA0);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 6
	patch.offset = 0x00001923;
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x0D);
	patch.patched_code.push_back(0x38);
	patch.patched_code.push_back(0x1D);
	patch.patched_code.push_back(0x96);
	patch.patched_code.push_back(0x00);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 7
	patch.offset = 0x0000198B;
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x0D);
	patch.patched_code.push_back(0x38);
	patch.patched_code.push_back(0x1D);
	patch.patched_code.push_back(0x96);
	patch.patched_code.push_back(0x00);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x11);
	patch.patched_code.push_back(0x8D);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xFA);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 8
	patch.offset = 0x00001B70;
	patch.patched_code.push_back(0x8D);
	patch.patched_code.push_back(0x4D);
	patch.patched_code.push_back(0xE8);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 9
	patch.offset = 0x00001C12;
	patch.patched_code.push_back(0x75);
	patch.patched_code.push_back(0x50);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 10
	patch.offset = 0x00001C40;
	patch.patched_code.push_back(0x8D);
	patch.patched_code.push_back(0x4D);
	patch.patched_code.push_back(0xE8);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 11
	patch.offset = 0x0000219D;
	patch.patched_code.push_back(0x75);
	patch.patched_code.push_back(0xCC);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 12
	patch.offset = 0x0000228A;
	patch.patched_code.push_back(0x73);
	patch.patched_code.push_back(0x0E);

	patches.push_back(patch);
	patch.patched_code.clear();

	//fix 13
	patch.offset = 0x00002414;
	patch.patched_code.push_back(0x89);
	patch.patched_code.push_back(0x4D);
	patch.patched_code.push_back(0xE4);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x16);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x42);
	patch.patched_code.push_back(0x04);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 14
	patch.offset = 0x000024BA;
	patch.patched_code.push_back(0xB8);
	patch.patched_code.push_back(0xC0);
	patch.patched_code.push_back(0x24);
	patch.patched_code.push_back(0x40);
	patch.patched_code.push_back(0x00);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 16
	patch.offset = 0x000024F7;
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xC4);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x08);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x51);
	patch.patched_code.push_back(0x04);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x44);
	patch.patched_code.push_back(0x02);
	patch.patched_code.push_back(0x28);
	patch.patched_code.push_back(0x85);
	patch.patched_code.push_back(0xC0);
	patch.patched_code.push_back(0x74);
	patch.patched_code.push_back(0x09);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 17
	patch.offset = 0x000025DC;
	patch.patched_code.push_back(0x89);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xCC);
	patch.patched_code.push_back(0x89);
	patch.patched_code.push_back(0x5D);
	patch.patched_code.push_back(0xC8);
	patch.patched_code.push_back(0x3B);
	patch.patched_code.push_back(0xC3);
	patch.patched_code.push_back(0x75);
	patch.patched_code.push_back(0x26);

	patches.push_back(patch);
	patch.patched_code.clear();

	
	//fix 18
	patch.offset = 0x000026BC;
	patch.patched_code.push_back(0xB8);
	patch.patched_code.push_back(0xC2);
	patch.patched_code.push_back(0x26);
	patch.patched_code.push_back(0x40);
	patch.patched_code.push_back(0x00);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 19
	patch.offset = 0x000027AE;
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x0D);
	patch.patched_code.push_back(0x24);
	patch.patched_code.push_back(0x82);
	patch.patched_code.push_back(0x86);
	patch.patched_code.push_back(0x00);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 20
	patch.offset = 0x000027EA;
	patch.patched_code.push_back(0x73);
	patch.patched_code.push_back(0x0E);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 21
	patch.offset = 0x0000281E;
	patch.patched_code.push_back(0x85);
	patch.patched_code.push_back(0xDB);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 22
	patch.offset = 0x000028DA
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x45);
	patch.patched_code.push_back(0xB8);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x08);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x51);
	patch.patched_code.push_back(0x04);
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x44);
	patch.patched_code.push_back(0x02);
	patch.patched_code.push_back(0x28);
	patch.patched_code.push_back(0x85);
	patch.patched_code.push_back(0xC0);
	patch.patched_code.push_back(0x74);
	patch.patched_code.push_back(0x09);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 23
	patch.offset = 0x000029B9;
	patch.patched_code.push_back(0x74);
	patch.patched_code.push_back(0x15);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 24
	patch.offset = 0x00002A38;
	patch.patched_code.push_back(0x33);
	patch.patched_code.push_back(0xC0);

	patches.push_back(patch);
	patch.patched_code.clear();


	//fix 25
	patch.offset = 0x0000330D;
	patch.patched_code.push_back(0x8B);
	patch.patched_code.push_back(0x0D);
	patch.patched_code.push_back(0x58);
	patch.patched_code.push_back(0x1D);
	patch.patched_code.push_back(0x96);
	patch.patched_code.push_back(0x00);

	patches.push_back(patch);
	patch.patched_code.clear();

}
