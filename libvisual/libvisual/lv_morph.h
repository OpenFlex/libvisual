/* Libvisual - The audio visualisation framework.
 *
 * Copyright (C) 2012      Libvisual team
 *               2004-2006 Dennis Smit
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef _LV_MORPH_H
#define _LV_MORPH_H

#include <libvisual/lv_audio.h>
#include <libvisual/lv_palette.h>
#include <libvisual/lv_plugin.h>
#include <libvisual/lv_video.h>
#include <libvisual/lv_time.h>

/**
 * @defgroup VisMorph VisMorph
 * @{
 */

#define VISUAL_MORPH(obj)               (VISUAL_CHECK_CAST ((obj), VisMorph))
#define VISUAL_MORPH_PLUGIN(obj)        (VISUAL_CHECK_CAST ((obj), VisMorphPlugin))

/**
 * Morph morphing methods.
 */
typedef enum {
    VISUAL_MORPH_MODE_SET,      /**< Morph progress is directly set */
    VISUAL_MORPH_MODE_TIME      /**< Morph progress is automatically
                                     measured and set using real time */
} VisMorphMode;

typedef struct _VisMorph VisMorph;
typedef struct _VisMorphPlugin VisMorphPlugin;

/**
 * Function type signature for the VisMorph palette() method.
 *
 * @param plugin   Plugin object
 * @param progress Morph progress (must be in [0.0, 1.0])
 * @param audio    Audio data
 * @param palette  Palette generated by LV
 * @param src1     First source Video
 * @param src2     Second source Video
 *
 * @return 0 on success, -1 on error.
 */
typedef int (*VisPluginMorphPaletteFunc) (VisPluginData *plugin,
                                          float          progress,
                                          VisAudio      *audio,
                                          VisPalette    *palette,
                                          VisVideo      *src1,
                                          VisVideo      *src2);

/**
 * Function type signature for the VisMorph apply() method.
 *
 * @param plugin   Plugin object
 * @param progress Morph progress (must be in [0.0 and 1])
 * @param audio    Audio data
 * @param src1     First source Video
 * @param src2     Second source Video
 *
 * @return 0 on success, -1 on error.
 */
typedef int (*VisPluginMorphApplyFunc) (VisPluginData *plugin,
                                        float          progress,
                                        VisAudio      *audio,
                                        VisVideo      *dest,
                                        VisVideo      *src1,
                                        VisVideo      *src2);

/**
 * Morph class.
 */
struct _VisMorph;

/**
 * Morph plugin class.
 */
struct _VisMorphPlugin {
    VisObject                 object;         /**< Parent  */
    VisPluginMorphPaletteFunc palette;        /**< Returns the color palette (only for 8-bit morphs) */
    VisPluginMorphApplyFunc   apply;          /**< Renders the morph */
    int                       requests_audio; /**< Flag indicating if morph plugin uses audio data */
    VisVideoAttrOptions       vidoptions;
};

LV_BEGIN_DECLS

/**
 * Returns the plugin backing the morph object.
 *
 * @param morph Morph object
 *
 * @return Plugin object, or NULL if none
 */
LV_API VisPluginData *visual_morph_get_plugin (VisMorph *morph);

/**
 * Returns the name of the next available morph plugin.
 *
 * @see visual_morph_get_prev_by_name()
 *
 * @param name Name of the current plugin, or NULL to retrieve the first.
 *
 * @return The name of the next plugin within the list.
 */
LV_API const char *visual_morph_get_next_by_name (const char *name);

/**
 * Returns the name of the previous available morph plugin.
 *
 * @see visual_morph_get_next_by_name()
 *
 * @param name Name of the current plugin. or NULL to retrieve the last.
 *
 * @return The name of the previous plugin within the list.
 */
LV_API const char *visual_morph_get_prev_by_name (const char *name);

/**
 * Creates a new morph object from name.
 *
 * @param name Name of morph plugin, or NULL to create an empty object.
 *
 * @see visual_morph_realize()
 *
 * @return A new morph object, or NULL on failure
 */
LV_API VisMorph *visual_morph_new (const char *name);

/**
 * Realizes a morph.
 *
 * This also calls the plugin init() method.
 *
 * @param morph Morph object
 *
 * @return VISUAL_OK on success
 */
LV_API int visual_morph_realize (VisMorph *morph);

/**
 * Returns a morph's supported colour depths.
 *
 * @param morph Morph object
 *
 * @return Supported depths
 */
LV_API VisVideoDepth visual_morph_get_supported_depth (VisMorph *morph);

LV_API VisVideoAttrOptions *visual_morph_get_video_attribute_options (VisMorph *morph);

/**
 * Sets the video rendering target for a morph.
 *
 * @param morph Morph object
 * @param video Video target
 */
LV_API void visual_morph_set_video (VisMorph *morph, VisVideo *video);

/**
 * Sets the duration of a morph.
 *
 * @param morph Morph object
 * @param time  Duration
 */
LV_API void visual_morph_set_time (VisMorph *morph, VisTime *time);

/**
 * Sets the progress of a morph.
 *
 * @param morph    Morph object
 * @param progress Progress of morph (must be in [0.0, 1.0])
 */
LV_API void visual_morph_set_progress (VisMorph *morph, float progress);

/**
 * Used to set the method of morphing.
 *
 * @param morph Pointer to a VisMorph to which the method of morphing is set.
 * @param mode Method of morphing that is of type VisMorphMode.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_MORPH_NULL on failure.
 */
LV_API void visual_morph_set_mode (VisMorph *morph, VisMorphMode mode);

/**
 * Returns the palette used by morph.
 *
 * @param morph Morph object.
 *
 * @note Only 8-bit morphs will have colour palettes.
 *
 * @return Palette object, or NULL if morph uses none
 */
LV_API VisPalette *visual_morph_get_palette (VisMorph *morph);

/**
 * Determines if morph has completed.
 *
 * @param morph Morph object
 *
 * @return TRUE if completed, FALSE otherwise
 */
LV_API int visual_morph_is_done (VisMorph *morph);

/**
 * Determines if morph requires audio data.
 *
 * @param morph Morph object
 *
 * @return TRUE if yes, FALSE otherwise
 */
LV_API int visual_morph_requests_audio (VisMorph *morph);

/**
 * Runs the morph.
 *
 * The rendering will be done onto the Video set with visual_morph_set_video().
 *
 * @note that all the VisVideo structures being used need to be clones.
 *
 * @param morph Morph object
 * @param audio Audio data
 * @param src1  First source Video
 * @param src2  Second source Video
 *
 * @return TRUE on success, FALSE otherwise
 */
LV_API int visual_morph_run (VisMorph *morph, VisAudio *audio, VisVideo *src1, VisVideo *src2);

LV_END_DECLS

/**
 * @}
 */

#endif /* _LV_MORPH_H */
