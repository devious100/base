// 6/20/2003 - Created by Vertigo Software, Inc.
// 11/17/2007 - Modified by Greg Dolley.

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Drawing::Drawing2D;
using namespace System::Reflection;

#include "Radar.h"
//#include "_vcclrit.h" // ***GREGS_VC9_PORT_MOD*** -- This header is obsolete because the mixed DLL loading problem does not apply to this version of MSVC++ and this CLR
#include <vcclr.h>

// using icon
#undef GetObject


namespace Radar
{
	// Radar window that displays the position of items in the Quake world.
	private ref class RadarForm : public System::Windows::Forms::Form // ***GREGS_VC9_PORT_MOD*** -- replaced "__gc" with "ref"
	{
	private:
		// consts used when drawing
		value struct Const // ***GREGS_VC9_PORT_MOD*** -- replaced "__value" with "value"
		{
			// radar
			static const int Scale = 6;
			static const int MonsterSize = 10;
			static const int CenterSize = 6;
			static const int ShadowSize = 2;
			static const int PlayerSize = 30;
			static const int FieldOfView = 60;
			
			// gradient
			static Color GradientStartColor = Color::DimGray;
			static Color GradientEndColor = Color::Black;
			static float GradientAngle = 45.0f;
			
			// overlay
			static float Opacity = 0.7f;
			static Color TransparentColor = Color::Black;
			static Color OverlayMessageColor = Color::Khaki;
			static String ^OverlayMessage = "Overlay Off"; // ***GREGS_VC9_PORT_MOD*** -- removed "S" string prefix, replaced "*" with "^"
			static int OverlayMaxWidth = 320;
			static int OverlayMaxHeight = 240;
		};
	
		// visual options can change from the context menu
		//value enum Options
		enum class Options // ***GREGS_VC9_PORT_MOD*** -- changed from "value enum Options" to "enum class Options" - I don't know why "value" was used instead of "__value" but I guess old managed C++ syntax allowed "value" to be synonymous with "__value"
		{
			CenterArrow = 0,
			Crosshairs = 1,
			DistanceCircles = 2,
			FieldOfView = 3
		};

	// data members
	private:
		// used for double buffering	
		Bitmap ^m_bmp; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator

		// player information
		int m_x, m_y;
		float m_angle;

		// list of radar items
		ItemVector* m_items;

		// visual options		
		//bool m_options __nogc[4]; // ***GREGS_VC9_PORT_MOD*** -- don't know why this had __nogc originally
		array<bool> ^m_options; // ***GREGS_VC9_PORT_MOD*** -- I removed __nogc, added "array<bool> ^" (I don't know why this had to be a non-managed array, but if this class doesn't work anymore, well, I'll know then)
		
		// gdi objects
		Pen ^m_penPlayer; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		Pen ^m_penDistance; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		AdjustableArrowCap ^m_cap; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		LinearGradientBrush ^m_brushBack; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		SolidBrush ^m_brushView; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator

		// overlay mode
		bool m_overlay;
		/*HWND*/ IntPtr m_hParent; // ***GREGS_VC9_PORT_MOD*** -- changed HWND to IntPtr to get rid of unmanaged pointer (this causes problems when other .NET modules link to this project)
		Drawing::Rectangle m_orgBounds;
		Drawing::Font ^m_overlayFont; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		Drawing::Rectangle m_overlayArea;

		// form objects		
		private: System::Windows::Forms::StatusBar ^statusBar; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::ContextMenu ^optionsMenu; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::MenuItem ^menuItem1; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::MenuItem ^menuItem2; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::MenuItem ^menuItem3; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::MenuItem ^menuItem4; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::StatusBarPanel ^statusBarPanel1; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::StatusBarPanel ^statusBarPanel2; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::MenuItem ^menuItem5; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
		private: System::Windows::Forms::MenuItem ^menuTransparent; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
	
	private:
		// Required designer variable.
		System::ComponentModel::Container ^components; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator

		// Required method for Designer support - do not modify
		// the contents of this method with the code editor.
		void InitializeComponent(void)
		{
         System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(RadarForm::typeid));
         this->optionsMenu = (gcnew System::Windows::Forms::ContextMenu());
         this->menuItem1 = (gcnew System::Windows::Forms::MenuItem());
         this->menuItem2 = (gcnew System::Windows::Forms::MenuItem());
         this->menuItem3 = (gcnew System::Windows::Forms::MenuItem());
         this->menuItem4 = (gcnew System::Windows::Forms::MenuItem());
         this->menuItem5 = (gcnew System::Windows::Forms::MenuItem());
         this->menuTransparent = (gcnew System::Windows::Forms::MenuItem());
         this->statusBar = (gcnew System::Windows::Forms::StatusBar());
         this->statusBarPanel1 = (gcnew System::Windows::Forms::StatusBarPanel());
         this->statusBarPanel2 = (gcnew System::Windows::Forms::StatusBarPanel());
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->statusBarPanel1))->BeginInit();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->statusBarPanel2))->BeginInit();
         this->SuspendLayout();
         // 
         // optionsMenu
         // 
         this->optionsMenu->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(6) {this->menuItem1, this->menuItem2, 
            this->menuItem3, this->menuItem4, this->menuItem5, this->menuTransparent});
         this->optionsMenu->Popup += gcnew System::EventHandler(this, &RadarForm::optionsMenu_Popup);
         // 
         // menuItem1
         // 
         this->menuItem1->Index = 0;
         this->menuItem1->Text = L"Show Center Arrow";
         this->menuItem1->Click += gcnew System::EventHandler(this, &RadarForm::optionsMenuClick);
         // 
         // menuItem2
         // 
         this->menuItem2->Index = 1;
         this->menuItem2->Text = L"Show Crosshairs";
         this->menuItem2->Click += gcnew System::EventHandler(this, &RadarForm::optionsMenuClick);
         // 
         // menuItem3
         // 
         this->menuItem3->Index = 2;
         this->menuItem3->Text = L"Show Distance Circles";
         this->menuItem3->Click += gcnew System::EventHandler(this, &RadarForm::optionsMenuClick);
         // 
         // menuItem4
         // 
         this->menuItem4->Index = 3;
         this->menuItem4->Text = L"Show Field of View";
         this->menuItem4->Click += gcnew System::EventHandler(this, &RadarForm::optionsMenuClick);
         // 
         // menuItem5
         // 
         this->menuItem5->Index = 4;
         this->menuItem5->Text = L"-";
         // 
         // menuTransparent
         // 
         this->menuTransparent->Index = 5;
         this->menuTransparent->ShowShortcut = false;
         this->menuTransparent->Text = L"Overlay on Quake";
         this->menuTransparent->Click += gcnew System::EventHandler(this, &RadarForm::menuTransparent_Click);
         // 
         // statusBar
         // 
         this->statusBar->Location = System::Drawing::Point(0, 280);
         this->statusBar->Name = L"statusBar";
         this->statusBar->Panels->AddRange(gcnew cli::array< System::Windows::Forms::StatusBarPanel^  >(2) {this->statusBarPanel1, 
            this->statusBarPanel2});
         this->statusBar->ShowPanels = true;
         this->statusBar->Size = System::Drawing::Size(280, 22);
         this->statusBar->TabIndex = 1;
         // 
         // statusBarPanel1
         // 
         this->statusBarPanel1->AutoSize = System::Windows::Forms::StatusBarPanelAutoSize::Spring;
         this->statusBarPanel1->Name = L"statusBarPanel1";
         this->statusBarPanel1->Text = L"Pos 0000, 0000, 0000";
         this->statusBarPanel1->Width = 163;
         // 
         // statusBarPanel2
         // 
         this->statusBarPanel2->Name = L"statusBarPanel2";
         // 
         // RadarForm
         // 
         this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
         this->ClientSize = System::Drawing::Size(280, 302);
         this->ContextMenu = this->optionsMenu;
         this->ControlBox = false;
         this->Controls->Add(this->statusBar);
         this->Enabled = false;
         this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
         this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
         this->MaximizeBox = false;
         this->MinimizeBox = false;
         this->Name = L"RadarForm";
         this->ShowInTaskbar = false;
         this->Text = L"Quake Radar";
         this->Load += gcnew System::EventHandler(this, &RadarForm::RadarForm_Load);
         this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &RadarForm::RadarForm_MouseDown);
         this->Closing += gcnew System::ComponentModel::CancelEventHandler(this, &RadarForm::RadarForm_Closing);
         this->Resize += gcnew System::EventHandler(this, &RadarForm::RadarForm_Resize);
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->statusBarPanel1))->EndInit();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->statusBarPanel2))->EndInit();
         this->ResumeLayout(false);

      }		
	
		
	public: 
		// ctor
		RadarForm(/*HWND*/ IntPtr hParent) // ***GREGS_VC9_PORT_MOD*** -- changed HWND to IntPtr to get rid of unmanaged pointer (this causes problems when other .NET modules link to this project)
		{
			// init data members
			m_items = NULL;
			m_hParent = hParent;
         
			// init options
			m_options = gcnew array<bool>(4); // ***GREGS_VC9_PORT_MOD*** -- added this line in order to initialize the size of the former native "bool m_options[4]" array - under Managed Extensions for C++ .NET 2005 you can no longer have a native type in a managed class (so I had to change it to a managed array); note: however, you _can_ have a tracking reference to a native type in a managed class, that _is_ OK
			m_options[0] = m_options[1] = m_options[2] = m_options[3] = true;

			CreateGdiObjects();
			
			InitializeComponent();
         SetOverlayMode(true); // ***TEST***
		}

		// dtor		
		//~RadarForm()
		!RadarForm() // ***GREGS_VC9_PORT_MOD*** -- destructor had to change to the finalizer under Managed Extensions for C++ .NET 2005 semantics (the "!" operator is new and acts as the former Finalize()) - see http://msdn2.microsoft.com/en-us/library/ms235315(VS.80).aspx for more info
		{
			if (m_items)
				delete m_items;
		}
		
		//void Dispose(Boolean disposing)
		~RadarForm() // ***GREGS_VC9_PORT_MOD*** -- Dispose() method had to change to the destructor under Managed Extensions for C++ .NET 2005 semantics - see http://msdn2.microsoft.com/en-us/library/ms235315(VS.80).aspx for more info
		{
			if (/*disposing && */components) // ***GREGS_VC9_PORT_MOD*** -- don't have "disposing" variable anymore
			{
				delete components; // ***GREGS_VC9_PORT_MOD*** -- was formerly calling "object_here->Dispose();" but had to change to support destructor semantics that were massively modified in Managed Extensions for C++ .NET 2005 - see http://msdn2.microsoft.com/en-us/library/ms235315(VS.80).aspx for more info
			}
			//__super::Dispose(disposing); // ***GREGS_VC9_PORT_MOD*** -- don't need this line anymore because the compiler injects a line like this in the CLI anyway - see http://msdn2.microsoft.com/en-us/library/ms235315(VS.80).aspx for more info
		}

	// public methods
	public:
		// hide or show the radar window
		void Display(bool visible)
		{
			// just hide or show the window when in overlay mode
			// otherwise, fade in / out the window
			if (m_overlay)
				visible ? this->Show() : this->Hide();
			else
				FadeWindow(visible);
		}

		// update the items on the radar
		void UpdateRadar(int x, int y, float angle, ItemVector* items)
		{
			// store player information

			m_x = x;				
			m_y = y;				
			m_angle = angle;		
			
			// see if need to delete existing list
			if (m_items)
			{
				delete m_items;
				m_items = NULL;
			}
	
			// allocate room for radar items
			if (items != NULL)
				m_items = new ItemVector(*items);
	
			// update the radar
			Draw();
			
			// update the status bar
			UpdateStatus();
		}


		// the Quake window changed
		void WindowPosChanged(/*HWND*/ IntPtr hParent) // ***GREGS_VC9_PORT_MOD*** -- changed HWND to IntPtr to get rid of unmanaged pointer (this causes problems when other .NET modules link to this project)
		{
			// only care about this when in overlay mode
			if (m_overlay)
			{
				m_hParent = hParent;

				// calculate the position and size of the overlay window
				RECT rcClient;
				GetClientRect((HWND)m_hParent.ToPointer(), &rcClient); // ***GREGS_VC9_PORT_MOD*** -- used ToPointer() and HWND type cast to make conversion work
				POINT pt1, pt2;
				pt1.x = pt1.y = 0;
				pt2.x = rcClient.right;
				pt2.y = rcClient.bottom;
				::ClientToScreen((HWND)m_hParent.ToPointer(), &pt1); // ***GREGS_VC9_PORT_MOD*** -- used ToPointer() and HWND type cast to make conversion work
				::ClientToScreen((HWND)m_hParent.ToPointer(), &pt2); // ***GREGS_VC9_PORT_MOD*** -- used ToPointer() and HWND type cast to make conversion work
				
				// set position and size so the radar is 
				// on top of the Quake window client area
				this->SetBounds(
					pt2.x - Const::OverlayMaxWidth, pt1.y,
					Const::OverlayMaxWidth, Const::OverlayMaxHeight);
			}
		}


	// helper methods
	private:
		void CreateGdiObjects()
		{
			// player pen
			m_penPlayer = gcnew Pen(Color::Khaki, 4); // ***GREGS_VC9_PORT_MOD*** -- changed "new" to "gcnew"
			m_cap = gcnew AdjustableArrowCap(3,5, true); // ***GREGS_VC9_PORT_MOD*** -- changed "new" to "gcnew" 
			m_penPlayer->CustomEndCap = m_cap;

			// distance pen
			m_penDistance = gcnew Pen(Color::FromArgb(30,30,30)); // ***GREGS_VC9_PORT_MOD*** -- changed "new" to "gcnew"

			// field of view			
			m_brushView = gcnew SolidBrush(Color::FromArgb(30, 255,255,255)); // ***GREGS_VC9_PORT_MOD*** -- changed "new" to "gcnew"

			// overlay message
			m_overlayFont = gcnew Drawing::Font("arial", 8, Drawing::FontStyle::Bold); // ***GREGS_VC9_PORT_MOD*** -- removed "S" string prefix, changed "new" to "gcnew"
		}
		
		
		// update the statusbar with the position of the player (x, y, degrees facing)
		void UpdateStatus()
		{
		   statusBar->Panels[0]->Text =
			/*statusBar->Panels->Item[0]->Text = */
				String::Format("Pos {0}, {1}, {2}°", // ***GREGS_VC9_PORT_MOD*** -- removed "S" string prefix
				m_x.ToString(), m_y.ToString(), m_angle.ToString("F")); // ***GREGS_VC9_PORT_MOD*** -- removed "S" string prefix
		}
		

		void InitRadar()
		{
			// calculate size of radar area
			int width = this->ClientRectangle.Width;
			int height = this->ClientRectangle.Height - 
				(statusBar->Visible ? statusBar->Height : 0);
				
			if (width <= 0 || height <= 0)
				return;
			
			// create offscreen bitmap
			m_bmp = gcnew Bitmap(width, height); // ***GREGS_VC9_PORT_MOD*** -- changed "new" to "gcnew"
			
			// gradient brush used for background in window mode
			Drawing::Rectangle rc = Drawing::Rectangle(0, 0, width, height);
			m_brushBack = gcnew LinearGradientBrush(rc,
				Const::GradientStartColor, Const::GradientEndColor,
				Const::GradientAngle, false);	// ***GREGS_VC9_PORT_MOD*** -- changed "new" to "gcnew"
				
			// calculate the message area when run in overlay mode
			Graphics ^g = Graphics::FromImage(m_bmp); // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
			Drawing::Size size = g->MeasureString(Const::OverlayMessage, m_overlayFont).ToSize();
			m_overlayArea = Drawing::Rectangle(
				m_bmp->Width - size.Width - 5, 5,
				size.Width, size.Height);
			delete g; // ***GREGS_VC9_PORT_MOD*** -- was formerly calling "object_here->Dispose();" but had to change to support destructor semantics that were massively modified in Managed Extensions for C++ .NET 2005 - see http://msdn2.microsoft.com/en-us/library/ms235315(VS.80).aspx for more info
		}


		// update the radar
		void Draw()
		{		
			if (!m_overlay)
			{
				// draw on bitmap when in window mode
				Graphics ^g = Graphics::FromImage(m_bmp); // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
				DrawList(g);
				delete g; // ***GREGS_VC9_PORT_MOD*** -- was formerly calling "object_here->Dispose();" but had to change to support destructor semantics that were massively modified in Managed Extensions for C++ .NET 2005 - see http://msdn2.microsoft.com/en-us/library/ms235315(VS.80).aspx for more info
				Invalidate();
			}
			else
			{
				// draw right away when in overlay mode
				Graphics ^g = Graphics::FromHwnd(this->Handle); // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
				DrawList(g);
				delete g; // ***GREGS_VC9_PORT_MOD*** -- was formerly calling "object_here->Dispose();" but had to change to support destructor semantics that were massively modified in Managed Extensions for C++ .NET 2005 - see http://msdn2.microsoft.com/en-us/library/ms235315(VS.80).aspx for more info
			}
		}
		
		
		// draw items in list on the radar
		void DrawList(Graphics ^g) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
		{
			// set smoothing mode (looks better when items are rotated)
			g->SmoothingMode = SmoothingMode::AntiAlias;

			// calculate the center of the radar
			PointF center = PointF((float)m_bmp->Width/2, (float)m_bmp->Height/2);
			
			// background
			if (m_overlay)
			{
				// transparent background
				g->Clear(Const::TransparentColor);
				
				// most of the window is transparent so can't display the 
				// context menu, display text so user has something to click on
				g->DrawString(Const::OverlayMessage, m_overlayFont, Brushes::DimGray, 
					(float)m_overlayArea.Left, (float)m_overlayArea.Top);
				g->DrawString(Const::OverlayMessage, m_overlayFont, Brushes::DarkGray, 
					(float)m_overlayArea.Left-1, (float)m_overlayArea.Top-1);
			}	
			else
			{
				// use gradient background when in window mode
				g->Clear(Const::GradientStartColor);
				g->FillRectangle(m_brushBack, 0, 0, m_bmp->Width, m_bmp->Height);
			}

			// circle distance lines
			// Original: if (m_options[Options::DistanceCircles] && !m_overlay) 
			if (m_options[System::Convert::ToInt32(Enum::Format(Options::typeid, Options::DistanceCircles, "d"))] && !m_overlay) // ***GREGS_VC9_PORT_MOD*** -- changed "m_options[Options::DistanceCircles]" to properly do the type conversion (enum to int) since now VC++ is more strict (this amount of strictness on an enum I think is absurd)
			{
				Drawing::Rectangle rcDistance = Drawing::Rectangle(0, 0, m_bmp->Width, m_bmp->Height);
				rcDistance.Inflate(-rcDistance.Width / 10, -rcDistance.Height / 10);
				g->DrawEllipse(m_penDistance, rcDistance);
				
				rcDistance.Inflate(-rcDistance.Width / 4, -rcDistance.Height / 4);
				g->DrawEllipse(m_penDistance, rcDistance);
			}

			// crosshairs
			// Original: if (m_options[Options::Crosshairs])
			if (m_options[System::Convert::ToInt32(Enum::Format(Options::typeid, Options::Crosshairs, "d"))]) // ***GREGS_VC9_PORT_MOD*** -- changed "m_options[Options::Crosshairs]" to properly do the type conversion (enum to int) since now VC++ is more strict (this amount of strictness on an enum I think is absurd)
			{
				g->DrawLine(Pens::DimGray, 
					(int)center.X, 0, (int)center.X, m_bmp->Height);
					
				g->DrawLine(Pens::DimGray, 
					0, (int)center.Y, m_bmp->Width, (int)center.Y);
			}
			
			// field of view
			// Original: if (m_options[Options::FieldOfView] && !m_overlay)
			if (m_options[System::Convert::ToInt32(Enum::Format(Options::typeid, Options::FieldOfView, "d"))] && !m_overlay) // ***GREGS_VC9_PORT_MOD*** -- changed "m_options[Options::Crosshairs]" to properly do the type conversion (enum to int) since now VC++ is more strict (this amount of strictness on an enum I think is absurd)
			{
				g->FillPie(m_brushView, 0, 0, m_bmp->Width, m_bmp->Height, 
					-90 - (Const::FieldOfView/2), Const::FieldOfView);
			}
			
			// player
			// Original: if (m_options[Options::CenterArrow] && !m_overlay)
			if (m_options[System::Convert::ToInt32(Enum::Format(Options::typeid, Options::CenterArrow, "d"))] && !m_overlay) // ***GREGS_VC9_PORT_MOD*** -- changed "m_options[Options::Crosshairs]" to properly do the type conversion (enum to int) since now VC++ is more strict (this amount of strictness on an enum I think is absurd)
			{
				g->DrawLine(m_penPlayer, center.X, 
					center.Y+(Const::PlayerSize/2), center.X, 
					center.Y-(Const::PlayerSize/2));
			}
			
			// rotate items on radar 
			// subtract 90 degrees to make 0 degrees at 12:00 instead of 3:00
			Matrix ^m = gcnew Matrix(); // ***GREGS_VC9_PORT_MOD*** -- changed "new" to "gcnew", replaced native "*" indirection operator with managed "^" operator
			m->RotateAt(m_angle - 90, center, MatrixOrder::Append);
			g->Transform = m;			
			
			// rect that is used to draw each item
			Drawing::Rectangle rc = Drawing::Rectangle(0, 0, 
				Const::MonsterSize, Const::MonsterSize);

			// center of radar item
			Drawing::Rectangle rcCenter = Drawing::Rectangle(0, 0, 
				Const::CenterSize, Const::CenterSize);

			// shadow of radar item
			Drawing::Rectangle rcShadow = Drawing::Rectangle(0, 0, 
				Const::MonsterSize, Const::MonsterSize);
			
			if (m_items != NULL)
			{
				// draw each item in the list
				ItemVector::iterator i;
				for (i = m_items->begin(); i != m_items->end(); i++)
				{
					// calculate location on radar					
					rc.X = (int)center.X + ((*i).x/Const::Scale) - (Const::MonsterSize/2);
					rc.Y = (int)center.Y - ((*i).y/Const::Scale) - (Const::MonsterSize/2);
					
					// center
					rcCenter.X = rc.X + ((Const::MonsterSize - Const::CenterSize) / 2);
					rcCenter.Y = rc.Y + ((Const::MonsterSize - Const::CenterSize) / 2);
					
					// shadow
					rcShadow.X = rc.X + Const::ShadowSize;
					rcShadow.Y = rc.Y + Const::ShadowSize;
					
					switch ((*i).type)
					{
						case RadarTypeItems:
							g->FillRectangle(Brushes::Black, rcShadow);
							g->FillRectangle(Brushes::SteelBlue, rc);
							g->FillRectangle(Brushes::DarkSlateGray, rcCenter);
							break;

						case RadarTypeHealth:
							g->FillRectangle(Brushes::Black, rcShadow);
							g->FillRectangle(Brushes::Green, rc);
							g->FillRectangle(Brushes::DarkGreen, rcCenter);
							break;
							
						case RadarTypeDeadMonsters:
							g->DrawEllipse(Pens::Firebrick, rc);
							break;
						
						case RadarTypeMonster:
							g->FillEllipse(Brushes::Black, rcShadow);
							g->FillEllipse(Brushes::Firebrick, rc);
							break;

						case RadarTypePlayer:
							g->FillEllipse(Brushes::Black, rcShadow);
							g->FillEllipse(Brushes::DarkOrange, rc);
							break;

						// unknown types
						default:
							g->DrawEllipse(Pens::SteelBlue, rc);
							break;
					}	
				}
			}
		}


		// update the state of the context menu visual options
		void UpdateOptionMenu()
		{
			for (int i=0; i < 4; i++)
			{
				// Original: optionsMenu->MenuItems->Item[i]->Checked = m_options[i];
				optionsMenu->MenuItems[i]->Checked = m_options[i]; // ***GREGS_VC9_PORT_MOD*** -- changed from line above
				//optionsMenu->MenuItems->Item[i]->Enabled = !m_overlay;
				optionsMenu->MenuItems[i]->Enabled = !m_overlay; // ***GREGS_VC9_PORT_MOD*** -- changed from line above
			}
		}


		// turn on / off overlay mode
		void SetOverlayMode(bool overlay)
		{
			m_overlay = overlay;
			menuTransparent->Checked = m_overlay;

			if (overlay)
			{
				// save current window size so can restore it later
				m_orgBounds = this->Bounds;

				// setup window for overlay mode
				this->statusBar->Hide();
				this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None; // ***GREGS_VC9_PORT_MOD*** -- had to change from "FormBorderStyle::None" to fully qualified name
				this->TransparencyKey = Const::TransparentColor;
				this->Opacity = Const::Opacity;
				this->TopMost = true;

				// set window pos / size to Quake window				
				WindowPosChanged(m_hParent);
				
				// redraw the redar
				Draw();
			}
			else
			{
				// restore to window mode
				this->Hide();
				
				this->TopMost = false;
				this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow; // ***GREGS_VC9_PORT_MOD*** -- changed "FormBorderStyle::SizableToolWindow" to fully qualified name; I don't know why it didn't work by itself considering the correct reference is used at the top of this file
				this->statusBar->Show();
				this->TransparencyKey = Color::Empty;
				this->SetBounds(m_orgBounds.Left, m_orgBounds.Top,
					m_orgBounds.Width, m_orgBounds.Height);
					
				FadeWindow(true);
			}
		}	
		
		
		// fade window in / out
		void FadeWindow(bool visible)
		{
			if (visible)
			{
				// fade window in
				this->Opacity = 0;
				this->Show();
				
				while (this->Opacity < 1)
				{
					System::Threading::Thread::Sleep(30);
					Application::DoEvents();
					this->Opacity+= 0.2;
					this->Refresh();
				}
			}
			else
			{
				// fade window out
				while (this->Opacity > 0)
				{
					System::Threading::Thread::Sleep(30);
					Application::DoEvents();
					this->Opacity-= 0.2;
					this->Refresh();
				}
				this->Hide();
			}	
		}	


	// overrides
	protected: 		
		void OnPaint(PaintEventArgs ^e) new // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator, added "new" keyword at end of function opening (conforming to Managed Extensions for C++ .NET 2005 standards - no more assumed non-virtual overrides, you must use the "new" operator)
		{
			// double buffering when in window mode
			if (!m_overlay)
				e->Graphics->DrawImage(m_bmp, 0, 0);
		}
		
		
		void OnPaintBackground(PaintEventArgs ^pevent) new // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator, added "new" keyword at end of function opening (conforming to Managed Extensions for C++ .NET 2005 standards - no more assumed non-virtual overrides, you must use the "new" operator)
		{
			// don't pass to base class
		}		
		

	// message handlers
	private: 
		void RadarForm_Load(System::Object ^sender, System::EventArgs ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
		{
			// display version in status bar
			Version ^v = Assembly::GetExecutingAssembly()->GetName()->Version; // // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
			array<System::Object ^> ^o = {v->Major.ToString(), v->Minor.ToString(), v->Build.ToString()}; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator, changed to use array<> object
			this->statusBarPanel2->Text = String::Format("Ver {0}.{1}.{2}", o); // ***GREGS_VC9_PORT_MOD*** -- removed "S" string prefix

			// set back color of window so can make it transparent in overlay mode
			this->BackColor = Const::TransparentColor;

			// initialize the radar		
			InitRadar();
			Draw();
			UpdateStatus();
		}


		void optionsMenu_Popup(System::Object ^sender, System::EventArgs ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
		{
			// the context menu is being displayed, update the 
			// state of the menu items
			UpdateOptionMenu();
		}


		void optionsMenuClick(System::Object ^sender, System::EventArgs ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
		{
			// one of the visual option context menus was clicked
			// update the item in the options array and redraw the radar
			try
			{
				MenuItem ^item = safe_cast<MenuItem ^>(sender); // ***GREGS_VC9_PORT_MOD*** -- replaced "__try_cast" with "safe_cast", replaced native "*" indirection operator with managed "^" operator
				m_options[item->Index] = !m_options[item->Index];
				Draw();
			}
			catch (System::Exception ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
			{
				Diagnostics::Debug::WriteLine(e->Message);
			}
		}


		void menuTransparent_Click(System::Object ^sender, System::EventArgs ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
		{
			// toggle the overlay mode
			SetOverlayMode(!menuTransparent->Checked);
		}


		void RadarForm_Resize(System::Object ^sender, System::EventArgs ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
		{
			// the radar window was resized, reinitialize the drawing surface
			InitRadar();
			Draw();
		}
		
		
		void RadarForm_MouseDown(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
		{
			// only process when in overlay mode
			// see if clicked on the overlay message in the upper right corner
			if (m_overlay)
			{
				if (m_overlayArea.Contains(e->X, e->Y))
					SetOverlayMode(false);
			}
		}


		void RadarForm_Closing(System::Object ^sender, System::ComponentModel::CancelEventArgs ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with the new managed "^" operator
		{
			// prevent the user from closing the radar window
			e->Cancel = true;
		}
	};


	// Radar extension to the Quake engine. Displays the position of 
	// items in the Quake world.
	public ref class Console
	{
	// class members
	private:
		// the radar window
		static RadarForm ^_form; // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator

	// public methods
	public:
		// hide or show the radar window
		static void Display(bool visible, /*HWND*/ IntPtr hParent) // ***GREGS_VC9_PORT_MOD*** -- changed HWND to IntPtr to get rid of unmanaged pointer (this causes problems when other .NET modules link to this project)
		{
			if (visible)
			{
				// see if the window has been created yet
				if (_form == nullptr) // ***GREGS_VC9_PORT_MOD*** -- changed "NULL" to "nullptr"
					_form = gcnew RadarForm(hParent); // ***GREGS_VC9_PORT_MOD*** -- changed "new" to "gcnew"
				
				// show the window
				_form->Display(true);
			}
			else
			{
				// hide the window
				if (_form != nullptr) // ***GREGS_VC9_PORT_MOD*** -- changed "NULL" to "nullptr"
					_form->Display(false);
			}
		}


		// update the items on the radar
		static void Update(int x, int y, float angle, /*ItemVector*/void* items) // ***GREGS_VC9_PORT_MOD*** -- changed "ItemVector*" to "void*" in order to avoid having to convert ItemVector into a ref class - since this function is called outside this dll, and ItemVector is a native (unmanaged) type, the link will fail from any .NET project linking to this one
		{
			// only process if the window is visible
			if (_form != nullptr && _form->Visible) // ***GREGS_VC9_PORT_MOD*** -- changed "NULL" to "nullptr"
				_form->UpdateRadar(x, y, angle, (ItemVector *)items); // ***GREGS_VC9_PORT_MOD*** -- added the "(ItemVector *)" type cast (see comment at start of function for more info)
		}


		// notified that the Quake window position or size has changed
		static void WindowPosChanged(IntPtr hParent) // ***GREGS_VC9_PORT_MOD*** -- changed HWND to IntPtr to get rid of unmanaged pointer (this causes problems when other .NET modules link to this project)
		{
			// pass along to the radar window
			if (_form != nullptr) // ***GREGS_VC9_PORT_MOD*** -- changed "NULL" to "nullptr"
				_form->WindowPosChanged(hParent);
		}


	// public methods for mixed DLL loading issue
	// see http://support.microsoft.com/?id=814472 for more info
	public:
		static BOOL Initialize() 
		{
			try 
			{
				return TRUE; //__crt_dll_initialize(); // ***GREGS_VC9_PORT_MOD*** -- The mixed DLL loading problem does not apply to this version of MSVC++ and this CLR
			} 
			catch(System::Exception ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
			{
				Diagnostics::Debug::WriteLine(e->Message);
				return FALSE;
			}
		}
	
		static BOOL Terminate() 
		{
			try 
			{
				return TRUE; //__crt_dll_terminate(); // ***GREGS_VC9_PORT_MOD*** -- The mixed DLL loading problem does not apply to this version of MSVC++ and this CLR
			} 
			catch(System::Exception ^e) // ***GREGS_VC9_PORT_MOD*** -- replaced native "*" indirection operator with managed "^" operator
			{
				Diagnostics::Debug::WriteLine(e->Message);
				return FALSE;
			}
		}
	};	

}