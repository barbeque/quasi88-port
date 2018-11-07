/***********************************************************************
 * �����ե��å����� (�����ƥ���¸)
 *
 *	�ܺ٤ϡ� graph.h ����
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "quasi88.h"
#include "graph.h"
#include "device.h"

/************************************************************************/

/*#define	DEBUG_PRINTF*/


/* �ʲ��� static ���ѿ������ץ��������ѹ��Ǥ����Τǥ������Х��ˤ��Ƥ��� */

    int	use_hwsurface	= TRUE;		/* HW SURFACE ���Ȥ����ɤ���	*/
    int	use_doublebuf	= FALSE;	/* ���֥��Хåե����Ȥ����ɤ���	*/


/* �ʲ��ϡ� event.c �ʤɤǻ��Ѥ��롢 OSD �ʥ������Х��ѿ� */

    int	sdl_mouse_rel_move;		/* �ޥ������а�ư�̸��β�ǽ��	*/



/************************************************************************/

static	T_GRAPH_SPEC	graph_spec;		/* ���ܾ���		*/

static	int		graph_exist;		/* ���ǡ����������Ѥ�	*/
static	T_GRAPH_INFO	graph_info;		/* ���λ��Ρ����̾���	*/


/************************************************************************
 *	SDL�ν�����
 *	SDL�ν�λ
 ************************************************************************/

int	sdl_init(void)
{
    SDL_version libver;
    SDL_GetVersion(&libver);

    if (verbose_proc) {
	printf("Initializing SDL (%d.%d.%d) ... ",
	       libver.major, libver.minor, libver.patch); fflush(NULL);
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {

	if (verbose_proc) printf("Failed\n");
	fprintf(stderr, "SDL Error: %s\n", SDL_GetError());

	return FALSE;

    } else {

	if (verbose_proc) printf("OK\n");
	return TRUE;
    }
}

/************************************************************************/

void	sdl_exit(void)
{
    SDL_Quit();
}


/************************************************************************
 *	�����ե��å������ν�����
 *	�����ե��å�������ư��
 *	�����ե��å������ν�λ
 ************************************************************************/

static	char	sdl_vname[16];
static	int	sdl_depth;
static	int	sdl_byte_per_pixel;

static	SDL_Rect **sdl_mode;
static	Uint32	   sdl_mode_flags;


const T_GRAPH_SPEC	*graph_init(void)
{
    /*int	win_w, win_h;
    int	ful_w, ful_h;
    int i;
    const SDL_VideoInfo *vi;

    sdl_vname[0] = '\0';
    SDL_VideoDriverName(sdl_vname, sizeof(sdl_vname));
    vi = SDL_GetVideoInfo();

    if (verbose_proc) {
	printf("Initializing Graphic System (SDL:%s) ... \n", sdl_vname);
    }

    // �����٤ȡ��ԥ����뤢�����ΥХ��ȿ��������å�

    sdl_depth          = vi->vfmt->BitsPerPixel;
    sdl_byte_per_pixel = vi->vfmt->BytesPerPixel;

#if	defined(SUPPORT_16BPP) && defined(SUPPORT_32BPP)
    if        (sdl_byte_per_pixel == 2 ||
	       sdl_byte_per_pixel == 4) {
         ; // OK
    } else {
	sdl_depth          = 16;
	sdl_byte_per_pixel = 2;
    }
#elif	defined(SUPPORT_16BPP)
	sdl_depth          = 16;
	sdl_byte_per_pixel = 2;
#elif	defined(SUPPORT_32BPP)
	sdl_depth          = 32;
	sdl_byte_per_pixel = 4;
#endif



#ifdef	DEBUG_PRINTF
    printf("  <VideoInfo> %s\n", sdl_vname);
    printf("  hw_available  %d  ", vi->hw_available);
    printf("  wm_available  %d\n", vi->wm_available);
    printf("  blit_hw       %d  ", vi->blit_hw     );
    printf("  blit_hw_CC    %d  ", vi->blit_hw_CC  );
    printf("  blit_hw_A     %d\n", vi->blit_hw_A   );
    printf("  blit_sw       %d  ", vi->blit_sw     );
    printf("  blit_sw_CC    %d  ", vi->blit_sw_CC  );
    printf("  blit_sw_A     %d\n", vi->blit_sw_A   );
    printf("  blit_fill     %d  ", vi->blit_fill   );
    printf("  video_mem     %d\n", vi->video_mem   );
    printf("  palette       %p\n", vi->vfmt->palette       );
    printf("  BitsPerPixel  %2d  ", vi->vfmt->BitsPerPixel  );
    printf("  BytesPerPixel %2d\n", vi->vfmt->BytesPerPixel );
    printf("  Rmask   %8x  ", vi->vfmt->Rmask         );
    printf("  Gmask   %8x  ", vi->vfmt->Gmask         );
    printf("  Bmask   %8x  ", vi->vfmt->Bmask         );
    printf("  Amask   %8x\n", vi->vfmt->Amask         );
    printf("  Rshift        %2d  ", vi->vfmt->Rshift        );
    printf("  Gshift        %2d  ", vi->vfmt->Gshift        );
    printf("  Bshift        %2d  ", vi->vfmt->Bshift        );
    printf("  Ashift        %2d\n", vi->vfmt->Ashift        );
    printf("  Rloss          %d  ", vi->vfmt->Rloss         );
    printf("  Gloss          %d  ", vi->vfmt->Gloss         );
    printf("  Bloss          %d  ", vi->vfmt->Bloss         );
    printf("  Aloss          %d\n", vi->vfmt->Aloss         );
    printf("  colorkey       %x  ", vi->vfmt->colorkey      );
    printf("  alpha          %d\n", vi->vfmt->alpha         );
    printf("\n");
#endif

    // ���Ѳ�ǽ�ʥ������ɥ��Υ�������Ĵ�٤Ƥ���
    for (i = 0; i < 2; i++) {
	Uint32 flags = 0;
	int w, h;

	if (i == 0) flags = 0;			// 1���ܤϥ������ɥ���
	else        flags = SDL_FULLSCREEN;	// 2���ܤ������̤������å�

	if (use_hwsurface) flags |= SDL_HWPALETTE | SDL_HWSURFACE;
	else               flags |= SDL_HWPALETTE | SDL_SWSURFACE;

	if (use_doublebuf) flags |= SDL_DOUBLEBUF;

	// ���Ѳ�ǽ�ʺ��祵����������
	sdl_mode = SDL_ListModes(NULL, flags);

	if        (sdl_mode == (SDL_Rect**) 0) {	// ���⡼���Բ�
	    w = 0;
	    h = 0;
	} else if (sdl_mode == (SDL_Rect**)-1) {	// ���⡼�ɲ�
	    w = 10000;
	    h = 10000;
	} else {					// �⡼�ɤ������å�
	    w = sdl_mode[0]->w;					// �ǽ餬
	    h = sdl_mode[0]->h;					//   ������

#ifdef	DEBUG_PRINTF
	    {
	      int j;
	      for (j=0; sdl_mode[j]; j++)
		printf("  %sSize %3d:  %4d x %4d  (%.4f)\n",
		       (i==0) ? "Window" : "Fullscreen", j, sdl_mode[j]->w,
		       sdl_mode[j]->h, (double)sdl_mode[j]->w/sdl_mode[j]->h);
	    }
#endif
	}

	if (i == 0) { win_w = w;  win_h = h; }
	else        { ful_w = w;  ful_h = h; }

	sdl_mode_flags = flags;
    }
    // ���λ����ǡ� sdl_mode �ˤϡ������̻��Υ⡼�ɰ��������åȤ����Ƥ��롣
    // sdl_mode_flags �ˤϡ������̻��Υ⡼�ɤΥե饰�����åȤ����Ƥ��롣

    graph_spec.window_max_width      = win_w;
    graph_spec.window_max_height     = win_h;
    graph_spec.fullscreen_max_width  = ful_w;
    graph_spec.fullscreen_max_height = ful_h;
    graph_spec.forbid_status         = FALSE;
    graph_spec.forbid_half           = FALSE;

    if (verbose_proc)
	printf("  INFO:%dbpp(%dbyte), Maxsize=win(%d,%d),full(%d,%d)\n",
	       sdl_depth, sdl_byte_per_pixel, win_w, win_h, ful_w, ful_h);

    return &graph_spec;*/

    // FIXME: identify available video modes (SDL_GetDisplayMode)

    graph_spec.window_max_width = 640;
    graph_spec.window_max_height = 400;
    graph_spec.fullscreen_max_width = 0;
    graph_spec.fullscreen_max_height = 0;
    graph_spec.forbid_status         = FALSE;
    graph_spec.forbid_half           = FALSE;

    return &graph_spec;
}

/************************************************************************/

static	int	search_mode(int w, int h, double aspect);

static	SDL_Surface	*sdl_display;
static	SDL_Surface	*sdl_offscreen;
static  SDL_Texture *sdl_texture;
static  SDL_Window  *sdl_window;
static  SDL_Renderer* sdl_renderer;


const T_GRAPH_INFO	*graph_setup(int width, int height,
				     int fullscreen, double aspect)
{
    /*Uint32 flags;

    // �������ѹ��䡢�������ɥ������������ؤκݤϡ����� SDL_SetVideoMode() ��
    // �Ƥ֤����������˰�ö�ӥǥ����֥����ƥ�����λ������ɬ�פ������餷��(?)��
    // (�ӥǥ��ɥ饤�а�¸���� x11, windib, directx �ϡ���λ������)

    if (graph_exist) {
	if (verbose_proc) printf("Re-Initializing Graphic System (SDL:%s) ...",
				 sdl_vname);

	if ((graph_info.fullscreen == FALSE && fullscreen == FALSE) &&
	    (! (sdl_display->flags & SDL_FULLSCREEN))) {

	    // �������ɥ��Υ������ѹ����ϡ���λ��ɬ�פϤʤ����������ġ�
	    if (verbose_proc) printf("\n");

	} else {
	    SDL_QuitSubSystem(SDL_INIT_VIDEO);
	    graph_exist = FALSE;
	}
    }


    // VIDEO����ö��λ�����ʤ顢VIDEO�κƽ�����
    if (! SDL_WasInit(SDL_INIT_VIDEO)) {
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
	    if (verbose_proc) printf(" FAILED\n");
	    return NULL;
	}
	// VIDEO����ö��λ�����ȡ� sdl_mode ��̵���ˤʤ롣(�ǥХ����ˤ��롩)
	sdl_mode = SDL_ListModes(NULL, sdl_mode_flags);

		// sdl_mode �����Ƥ����������Ѥ��äƤ��ޤä����ɤ����褦��

	if (verbose_proc) printf(" OK\n");
    }


    // �����̥⡼�ɤξ��硢Ŭ�ڤʥ⡼�ɤ�����
    if (fullscreen) {
	int fit = search_mode(width, height, aspect);
	if (fit < 0) {
	    fullscreen = FALSE;
	} else {
	    width  = sdl_mode[fit]->w;
	    height = sdl_mode[fit]->h;
	}
    }

    // �������ɥ��������̡ˤ򳫤�
    if (verbose_proc) {
	if (fullscreen) printf("  Trying full screen mode ... ");
	else            printf("  Opening window ... ");
    }

    if (fullscreen) flags = SDL_FULLSCREEN;
    else            flags = 0;

    if (use_hwsurface) flags |= SDL_HWPALETTE | SDL_HWSURFACE;
    else               flags |= SDL_HWPALETTE | SDL_SWSURFACE;

    if (use_doublebuf) flags |= SDL_DOUBLEBUF;

    sdl_display = SDL_SetVideoMode(width, height, sdl_depth, flags);

    if (verbose_proc)
	printf("%s (%dx%d)\n", (sdl_display ? "OK" : "FAILED"), width, height);

    if (sdl_display == NULL) return NULL;


    // �����꡼���Хåե�������

    if (verbose_proc) printf("  Allocating screen buffer ... ");

    sdl_offscreen = SDL_CreateRGBSurface(SDL_SWSURFACE,
					 width, height, sdl_depth,
					 0, 0, 0, 0);
    if (verbose_proc) printf("%s\n", (sdl_offscreen ? "OK" : "FAILED"));

    if (sdl_offscreen == NULL) return NULL;



    // ���̾����򥻥åȤ��ơ��֤�

    graph_info.fullscreen	= fullscreen;
    graph_info.width		= sdl_offscreen->w;
    graph_info.height		= sdl_offscreen->h;
    graph_info.byte_per_pixel	= sdl_byte_per_pixel;
    graph_info.byte_per_line	= sdl_offscreen->pitch;
    graph_info.buffer		= sdl_offscreen->pixels;
    graph_info.nr_color		= 255;
    graph_info.write_only	= FALSE;
    graph_info.broken_mouse	= FALSE;
    graph_info.draw_start	= NULL;
    graph_info.draw_finish	= NULL;
    graph_info.dont_frameskip	= FALSE;

    graph_exist = TRUE;

    if (verbose_proc)
	printf("    VideoMode %dx%d -> %dx%dx%d(%d)  %c%c%c%c  R:%x G:%x B:%x\n",
	       width, height, sdl_display->w, sdl_display->h,
	       sdl_display->format->BitsPerPixel,
	       sdl_display->format->BytesPerPixel,
	       (sdl_display->flags & SDL_SWSURFACE) ? 'S' : '-',
	       (sdl_display->flags & SDL_HWSURFACE) ? 'H' : 'S',
	       (sdl_display->flags & SDL_DOUBLEBUF) ? 'D' : '-',
	       (sdl_display->flags & SDL_FULLSCREEN) ? 'F' : '-',
	       sdl_display->format->Rmask,
	       sdl_display->format->Gmask, sdl_display->format->Bmask);

#if 0	// debug
printf("@ fullscreen      %d\n",    graph_info.fullscreen    );
printf("@ width           %d\n",    graph_info.width         );
printf("@ height          %d\n",    graph_info.height        );
printf("@ byte_per_pixel  %d\n",    graph_info.byte_per_pixel);
printf("@ byte_per_line   %d\n",    graph_info.byte_per_line );
printf("@ buffer          %p\n",    graph_info.buffer        );
printf("@ nr_color        %d\n",    graph_info.nr_color      );
printf("@ write_only      %d\n",    graph_info.write_only    );
printf("@ broken_mouse    %d\n",    graph_info.broken_mouse  );
printf("@ dont_frameskip  %d\n",    graph_info.dont_frameskip);
#endif

    return &graph_info;*/

    int sdl_width = 640;
    int sdl_height = 400;

    // FIXME HACK just trying to get this damn thing to work
    sdl_window = SDL_CreateWindow("quasi88",
                  SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
                  sdl_width, sdl_height, 0);

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);

    sdl_offscreen = SDL_CreateRGBSurface(0, sdl_width, sdl_height, 32,
                      0x00ff0000,
                      0x0000ff00,
                      0x000000ff,
                      0xff000000);

    sdl_texture = SDL_CreateTexture(
                      sdl_renderer,
                      SDL_PIXELFORMAT_ARGB8888,
                      SDL_TEXTUREACCESS_STREAMING,
                      sdl_width, sdl_height);

    // HACKS
    sdl_byte_per_pixel = 4;

    graph_info.fullscreen	= FALSE;
    graph_info.width		= sdl_offscreen->w;
    graph_info.height		= sdl_offscreen->h;
    graph_info.byte_per_pixel	= sdl_byte_per_pixel;
    graph_info.byte_per_line	= sdl_offscreen->pitch;
    graph_info.buffer		= sdl_offscreen->pixels;
    graph_info.nr_color		= 255;
    graph_info.write_only	= FALSE;
    graph_info.broken_mouse	= FALSE;
    graph_info.draw_start	= NULL;
    graph_info.draw_finish	= NULL;
    graph_info.dont_frameskip	= FALSE;

    return &graph_info;
}



/*======================================================================*/
#define	FABS(a)		(((a) >= 0.0) ? (a) : -(a))

static	int	search_mode(int w, int h, double aspect)
{
    int i;
    int fit = -1;
    int fit_w = 0, fit_h = 0;
    double fit_a = 0.0;

    for (i=0; sdl_mode[i]; i++) {
	/* ���̥������˼��ޤäƤ��뤳�� */
	if (w <= sdl_mode[i]->w &&
	    h <= sdl_mode[i]->h) {

	    int tmp_w = sdl_mode[i]->w;
	    int tmp_h = sdl_mode[i]->h;
	    double tmp_a = FABS(((double)tmp_w / tmp_h) - aspect);

	    /* �ǽ��˸��Ĥ��ä����Τ��ޤ��ϥ��祤�� */
	    if (fit == -1) {
		fit = i;
		fit_w = tmp_w;
		fit_h = tmp_h;
		fit_a = tmp_a;

	    } else {
	    /* �������ϡ������Τ����٤ơ������ե��åȤ����Х��祤�� */

		/* ��Ĺ���˥������ʤ����������ڥ���̤�����ξ��� */
		if (aspect >= 1.0 || aspect < 0.01) {

		    /* �Ĥκ��ξ��ʤ��ۤ����ޤ��ϥ����ڥ������ζᤤ�ۤ� */
		    if (((tmp_h - h) < (fit_h - h)) ||
			((tmp_h == fit_h) && (tmp_a < fit_a))) {
			fit = i;
			fit_w = tmp_w;
			fit_h = tmp_h;
			fit_a = tmp_a;
		    }

		} else {	/* ��Ĺ���˥��� (�ʤ��ư���Ū�ʤ�?) �ξ��� */

		    /* ���κ��ξ��ʤ��ۤ����ޤ��ϥ����ڥ������ζᤤ�ۤ� */
		    if (((tmp_w - w) < (fit_w - w)) ||
			((tmp_w == fit_w) && (tmp_a < fit_a))) {
			fit = i;
			fit_w = tmp_w;
			fit_h = tmp_h;
			fit_a = tmp_a;
		    }
		}
	    }
	}
    }
    /* ���������Τ������ʤ������ϡ� -1 ���֤� */
    return fit;
}

/************************************************************************/

void	graph_exit(void)
{
    SDL_SetWindowGrab(sdl_window, FALSE);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


/************************************************************************
 *	���γ���
 *	���β���
 ************************************************************************/

void	graph_add_color(const PC88_PALETTE_T color[],
			int nr_color, unsigned long pixel[])
{
    int i;
    for (i=0; i<nr_color; i++) {
	pixel[i] = SDL_MapRGB(sdl_offscreen->format,
			      color[i].red, color[i].green, color[i].blue);
    }
}

/************************************************************************/

void	graph_remove_color(int nr_pixel, unsigned long pixel[])
{
    /* ���˴ؤ��Ƥϲ����������ʤ��Τǡ������Ǥ��ʤˤ⤷�ʤ� */
}



/************************************************************************
 *	�����ե��å��ι���
 ************************************************************************/

void	graph_update(int nr_rect, T_GRAPH_RECT rect[])
{
    SDL_Rect srect[16], drect;
    int i;

    if (nr_rect > 16) {
	fprintf(stderr, "SDL: Maybe Update Failied...\n");
	nr_rect = 16;
    }

    for (i=0; i<nr_rect; i++) {
	srect[i].x = rect[i].x;
	srect[i].y = rect[i].y;
	srect[i].w = rect[i].width;
	srect[i].h = rect[i].height;

	drect = srect[i];

	if (SDL_BlitSurface(sdl_offscreen, &srect[i], sdl_display, &drect) <0) {
	    fprintf(stderr, "SDL: Unsuccessful blitting to %i, %i (%i x %i)\n", srect[i].x, srect[i].y, srect[i].w, srect[i].h);
	}
    }

  // Copy the SDL_Surface into an SDL_Texture
  SDL_UpdateTexture(sdl_texture, NULL, sdl_offscreen->pixels, sdl_offscreen->pitch);

  // Write the texture to screen
  SDL_RenderClear(sdl_renderer);
  SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, NULL);

  // Flip
  SDL_RenderPresent(sdl_renderer);

  // FIXME: no idea what this piece of code was supposed to do

	/*for (i=0; i<nr_rect; i++) {
	    drect = srect[i];
	    SDL_BlitSurface(sdl_offscreen, &srect[i], sdl_display, &drect);
	}*/
}


/************************************************************************
 *	�����ȥ�������
 *	°��������
 ************************************************************************/

void	graph_set_window_title(const char *title)
{
    SDL_SetWindowTitle(sdl_window, title);
}

/************************************************************************/

void	graph_set_attribute(int mouse_show, int grab, int keyrepeat_on)
{
    if (mouse_show) SDL_ShowCursor(SDL_ENABLE);
    else            SDL_ShowCursor(SDL_DISABLE);

    if (grab) SDL_SetWindowGrab(sdl_window, TRUE);
    else      SDL_SetWindowGrab(sdl_window, FALSE);

    /*if (keyrepeat_on) SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
					  SDL_DEFAULT_REPEAT_INTERVAL);
    else              SDL_EnableKeyRepeat(0, 0);*/
    // FIXME ^

    sdl_mouse_rel_move = (mouse_show == FALSE && grab) ? TRUE : FALSE;

    /* SDL �ϡ��������椫�ĥޥ������դʤ顢�������ɥ���ü�˥ޥ�����
       �Ҥä����äƤ⡢�ޥ�����ư�������̤����ΤǤ��롣

       �ʤΤǡ����ξ����� sdl_mouse_rel_move �˥��åȤ��Ƥ�����
       ���ʤ顢�ޥ�����ư�������̡����ʤ����а��֤Ȥ��� (event.c)

       ���˥塼�Ǥϡ����ʤ餺�����֤ʤ� (�ޥ����Ϥ��� or �ʤ�) �ʤΤǡ�
       ���ξ����ˤϤ����餺�����˥������ɥ���ü�ǥޥ��������ߤ��롣
    */
}

/*
  -videodrv directx �ˤĤ���

  �����֤��ꡢ�ޥ��������ξ��硢�����֤����ʤ�������

  �����̤ǡ������֤ʤ����ޥ����ʤ��ˤ����ȡ�
  �ޥ��������̤�ü�����ߤ��Ƥ��ޤ����������ޤ�������
  �����̤ξ��硢�����֤ʤ��ϰ�̣�������Τ��� �ޥ����ǥ����ץ쥤�Ǹ���



  -videodrv dga �ˤĤ���

  �������ɥ��Ǥ������̤Ǥ⡢�����̥ե饰��Ω�äƤ��롣
  �ǥե����Ȥ� -hwsurface �ˤʤäƤ��롣 -swsurface �λ����ϲ�ǽ��
  -doublebuf �����ꤹ���ȡ� -hwsurface �⥻�åȤ�ͭ���ˤʤ롣

  �����̢����������ɥ��򷫤��֤��ȥ������Ǥ���

  -hwsurface �Ǥϡ��ޥ�����ɽ���˻����ĳ����Ĥ롣
  -swsurface �����ꤵ�ʤ���

  -doublebuf �����ꤹ���ȡ��ޥ�����ɽ�������ʤ��ʤ롣
*/
