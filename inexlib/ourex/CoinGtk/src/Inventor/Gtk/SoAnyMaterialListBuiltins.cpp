/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  The libraries this file is part of is free software; you can
 *  redistribute them and/or modify them under the terms of the GNU
 *  Lesser General Public License version 2.1 as published by the
 *  Free Software Foundation.  See the file LICENSE.LGPL at the root
 *  directory of the distribution for all the details.
 *
 *  If you want to use the Coin GUI binding libraries for applications
 *  not compatible with the LGPL, contact SIM about acquiring a
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// @configure_input@

#if SOGTK_DEBUG
static const char rcsid[] =
  "";
#endif // SOGTK_DEBUG

/**************************************************************************
 * IMPORTANT NOTICE
 * Be aware that the file SoGtkMaterialListBuiltins.cpp is generated from
 * the file SoGtkMaterialListBuiltins.cpp.m4, so changes has to be done in
 * the source file with the m4 macros.  The generation is done manually,
 * since it is no point in making this project depend on the user having
 * m4 installed.
 **************************************************************************/

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <string.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoAnyMaterialList.h>

#ifdef WITH_STATIC_DEFAULTS
#include <materials/materials.h>
#endif // WITH_STATIC_DEFAULTS

// *************************************************************************

#ifdef WITH_STATIC_DEFAULTS
static
const char *
builtin_material_names[] = {
  "artdeco",
    "artdeco.0",
    "artdeco.1",
    "artdeco.2",
    "artdeco.3",
    "artdeco.4",
    "artdeco.5",
    "artdeco.6",
    "artdeco.7",
    "artdeco.8",
    "artdeco.9",
    "artdeco.10",
    "artdeco.11",
    "artdeco.12",
    "artdeco.13",
    "artdeco.14",
    "artdeco.15",
    "artdeco.16",
    "artdeco.17",
    "artdeco.18",
    "artdeco.19",
    "artdeco.20",
    "artdeco.21",
    "artdeco.22",
    "artdeco.23",
    "artdeco.24",
    "artdeco.25",
    "artdeco.26",
    "artdeco.27",
    "artdeco.28",
    "artdeco.29",
    "artdeco.30",
    "artdeco.31",
    "artdeco.32",
    "artdeco.33",
    "artdeco.34",
  "autumn",
    "autumn.0",
    "autumn.1",
    "autumn.2",
    "autumn.3",
    "autumn.4",
    "autumn.5",
    "autumn.6",
    "autumn.7",
    "autumn.8",
    "autumn.9",
    "autumn.10",
    "autumn.11",
    "autumn.12",
    "autumn.13",
    "autumn.14",
    "autumn.15",
    "autumn.16",
    "autumn.17",
    "autumn.18",
    "autumn.19",
    "autumn.20",
    "autumn.21",
    "autumn.22",
    "autumn.23",
    "autumn.24",
    "autumn.25",
    "autumn.26",
    "autumn.27",
    "autumn.28",
    "autumn.29",
    "autumn.30",
    "autumn.31",
    "autumn.32",
    "autumn.33",
    "autumn.34",
  "glass",
    "glass.0",
    "glass.1",
    "glass.2",
    "glass.3",
    "glass.4",
    "glass.5",
    "glass.6",
    "glass.7",
    "glass.8",
    "glass.9",
    "glass.10",
    "glass.11",
    "glass.12",
    "glass.13",
    "glass.14",
    "glass.15",
    "glass.16",
    "glass.17",
    "glass.18",
    "glass.19",
    "glass.20",
    "glass.21",
    "glass.22",
    "glass.23",
    "glass.24",
    "glass.25",
    "glass.26",
    "glass.27",
    "glass.28",
    "glass.29",
    "glass.30",
    "glass.31",
    "glass.32",
    "glass.33",
    "glass.34",
  "metal",
    "metal.0",
    "metal.1",
    "metal.2",
    "metal.3",
    "metal.4",
    "metal.5",
    "metal.6",
    "metal.7",
    "metal.8",
    "metal.9",
    "metal.10",
    "metal.11",
    "metal.12",
    "metal.13",
    "metal.14",
    "metal.15",
    "metal.16",
    "metal.17",
    "metal.18",
    "metal.19",
    "metal.20",
    "metal.21",
    "metal.22",
    "metal.23",
    "metal.24",
    "metal.25",
    "metal.26",
    "metal.27",
    "metal.28",
    "metal.29",
    "metal.30",
    "metal.31",
    "metal.32",
    "metal.33",
    "metal.34",
  "neon",
    "neon.0",
    "neon.1",
    "neon.2",
    "neon.3",
    "neon.4",
    "neon.5",
    "neon.6",
    "neon.7",
    "neon.8",
    "neon.9",
    "neon.10",
    "neon.11",
    "neon.12",
    "neon.13",
    "neon.14",
    "neon.15",
    "neon.16",
    "neon.17",
    "neon.18",
    "neon.19",
    "neon.20",
    "neon.21",
    "neon.22",
    "neon.23",
    "neon.24",
    "neon.25",
    "neon.26",
    "neon.27",
    "neon.28",
    "neon.29",
    "neon.30",
    "neon.31",
    "neon.32",
    "neon.33",
    "neon.34",
  "rococo",
    "rococo.0",
    "rococo.1",
    "rococo.2",
    "rococo.3",
    "rococo.4",
    "rococo.5",
    "rococo.6",
    "rococo.7",
    "rococo.8",
    "rococo.9",
    "rococo.10",
    "rococo.11",
    "rococo.12",
    "rococo.13",
    "rococo.14",
    "rococo.15",
    "rococo.16",
    "rococo.17",
    "rococo.18",
    "rococo.19",
    "rococo.20",
    "rococo.21",
    "rococo.22",
    "rococo.23",
    "rococo.24",
    "rococo.25",
    "rococo.26",
    "rococo.27",
    "rococo.28",
    "rococo.29",
    "rococo.30",
    "rococo.31",
    "rococo.32",
    "rococo.33",
    "rococo.34",
  "santafe",
    "santafe.0",
    "santafe.1",
    "santafe.2",
    "santafe.3",
    "santafe.4",
    "santafe.5",
    "santafe.6",
    "santafe.7",
    "santafe.8",
    "santafe.9",
    "santafe.10",
    "santafe.11",
    "santafe.12",
    "santafe.13",
    "santafe.14",
    "santafe.15",
    "santafe.16",
    "santafe.17",
    "santafe.18",
    "santafe.19",
    "santafe.20",
    "santafe.21",
    "santafe.22",
    "santafe.23",
    "santafe.24",
    "santafe.25",
    "santafe.26",
    "santafe.27",
    "santafe.28",
    "santafe.29",
    "santafe.30",
    "santafe.31",
    "santafe.32",
    "santafe.33",
    "santafe.34",
  "sheen",
    "sheen.0",
    "sheen.1",
    "sheen.2",
    "sheen.3",
    "sheen.4",
    "sheen.5",
    "sheen.6",
    "sheen.7",
    "sheen.8",
    "sheen.9",
    "sheen.10",
    "sheen.11",
    "sheen.12",
    "sheen.13",
    "sheen.14",
    "sheen.15",
    "sheen.16",
    "sheen.17",
    "sheen.18",
    "sheen.19",
    "sheen.20",
    "sheen.21",
    "sheen.22",
    "sheen.23",
    "sheen.24",
    "sheen.25",
    "sheen.26",
    "sheen.27",
    "sheen.28",
    "sheen.29",
    "sheen.30",
    "sheen.31",
    "sheen.32",
    "sheen.33",
    "sheen.34",
  "silky",
    "silky.0",
    "silky.1",
    "silky.2",
    "silky.3",
    "silky.4",
    "silky.5",
    "silky.6",
    "silky.7",
    "silky.8",
    "silky.9",
    "silky.10",
    "silky.11",
    "silky.12",
    "silky.13",
    "silky.14",
    "silky.15",
    "silky.16",
    "silky.17",
    "silky.18",
    "silky.19",
    "silky.20",
    "silky.21",
    "silky.22",
    "silky.23",
    "silky.24",
    "silky.25",
    "silky.26",
    "silky.27",
    "silky.28",
    "silky.29",
    "silky.30",
    "silky.31",
    "silky.32",
    "silky.33",
    "silky.34",
  "spring",
    "spring.0",
    "spring.1",
    "spring.2",
    "spring.3",
    "spring.4",
    "spring.5",
    "spring.6",
    "spring.7",
    "spring.8",
    "spring.9",
    "spring.10",
    "spring.11",
    "spring.12",
    "spring.13",
    "spring.14",
    "spring.15",
    "spring.16",
    "spring.17",
    "spring.18",
    "spring.19",
    "spring.20",
    "spring.21",
    "spring.22",
    "spring.23",
    "spring.24",
    "spring.25",
    "spring.26",
    "spring.27",
    "spring.28",
    "spring.29",
    "spring.30",
    "spring.31",
    "spring.32",
    "spring.33",
    "spring.34",
  "summer",
    "summer.0",
    "summer.1",
    "summer.2",
    "summer.3",
    "summer.4",
    "summer.5",
    "summer.6",
    "summer.7",
    "summer.8",
    "summer.9",
    "summer.10",
    "summer.11",
    "summer.12",
    "summer.13",
    "summer.14",
    "summer.15",
    "summer.16",
    "summer.17",
    "summer.18",
    "summer.19",
    "summer.20",
    "summer.21",
    "summer.22",
    "summer.23",
    "summer.24",
    "summer.25",
    "summer.26",
    "summer.27",
    "summer.28",
    "summer.29",
    "summer.30",
    "summer.31",
    "summer.32",
    "summer.33",
    "summer.34",
  "tropical",
    "tropical.0",
    "tropical.1",
    "tropical.2",
    "tropical.3",
    "tropical.4",
    "tropical.5",
    "tropical.6",
    "tropical.7",
    "tropical.8",
    "tropical.9",
    "tropical.10",
    "tropical.11",
    "tropical.12",
    "tropical.13",
    "tropical.14",
    "tropical.15",
    "tropical.16",
    "tropical.17",
    "tropical.18",
    "tropical.19",
    "tropical.20",
    "tropical.21",
    "tropical.22",
    "tropical.23",
    "tropical.24",
    "tropical.25",
    "tropical.26",
    "tropical.27",
    "tropical.28",
    "tropical.29",
    "tropical.30",
    "tropical.31",
    "tropical.32",
    "tropical.33",
    "tropical.34",
  "winter",
    "winter.0",
    "winter.1",
    "winter.2",
    "winter.3",
    "winter.4",
    "winter.5",
    "winter.6",
    "winter.7",
    "winter.8",
    "winter.9",
    "winter.10",
    "winter.11",
    "winter.12",
    "winter.13",
    "winter.14",
    "winter.15",
    "winter.16",
    "winter.17",
    "winter.18",
    "winter.19",
    "winter.20",
    "winter.21",
    "winter.22",
    "winter.23",
    "winter.24",
    "winter.25",
    "winter.26",
    "winter.27",
    "winter.28",
    "winter.29",
    "winter.30",
    "winter.31",
    "winter.32",
    "winter.33",
    "winter.34",
  NULL
}; // builtin_material_names
#endif // WITH_STATIC_DEFAULTS

// *************************************************************************

/*!
  \internal

  This method fills in the SoGtkMaterialDirectory structure with the builtin
  material data.
*/

void
SoAnyMaterialList::setupBuiltinMaterials(// private
  SoGtkMaterialDirectory * const index) const
{
  assert(index != NULL);
  index->numGroups = 0;
  index->groups = NULL;
#ifdef WITH_STATIC_DEFAULTS
  index->flags |= SOGTK_BUILTIN_MATERIALS;
  index->numGroups = 13;
  index->groups = new SoGtkMaterialGroup * [ 13 ];
  for (int i = 0; i < index->numGroups; i++) {
    index->groups[i] = new SoGtkMaterialGroup;
    index->groups[i]->numMaterials = 35;
    index->groups[i]->materials = new SoGtkMaterial * [ 35 ];
    for (int j = 0; j < 35; j++) {
      index->groups[i]->materials[j] = new SoGtkMaterial;
    }
  }

  index->groups[0]->name = builtin_material_names[0];
  index->groups[0]->materials[0]->name = builtin_material_names[1];
  index->groups[0]->materials[0]->data = artdeco0_iv;
  index->groups[0]->materials[1]->name = builtin_material_names[2];
  index->groups[0]->materials[1]->data = artdeco1_iv;
  index->groups[0]->materials[2]->name = builtin_material_names[3];
  index->groups[0]->materials[2]->data = artdeco2_iv;
  index->groups[0]->materials[3]->name = builtin_material_names[4];
  index->groups[0]->materials[3]->data = artdeco3_iv;
  index->groups[0]->materials[4]->name = builtin_material_names[5];
  index->groups[0]->materials[4]->data = artdeco4_iv;
  index->groups[0]->materials[5]->name = builtin_material_names[6];
  index->groups[0]->materials[5]->data = artdeco5_iv;
  index->groups[0]->materials[6]->name = builtin_material_names[7];
  index->groups[0]->materials[6]->data = artdeco6_iv;
  index->groups[0]->materials[7]->name = builtin_material_names[8];
  index->groups[0]->materials[7]->data = artdeco7_iv;
  index->groups[0]->materials[8]->name = builtin_material_names[9];
  index->groups[0]->materials[8]->data = artdeco8_iv;
  index->groups[0]->materials[9]->name = builtin_material_names[10];
  index->groups[0]->materials[9]->data = artdeco9_iv;
  index->groups[0]->materials[10]->name = builtin_material_names[11];
  index->groups[0]->materials[10]->data = artdeco10_iv;
  index->groups[0]->materials[11]->name = builtin_material_names[12];
  index->groups[0]->materials[11]->data = artdeco11_iv;
  index->groups[0]->materials[12]->name = builtin_material_names[13];
  index->groups[0]->materials[12]->data = artdeco12_iv;
  index->groups[0]->materials[13]->name = builtin_material_names[14];
  index->groups[0]->materials[13]->data = artdeco13_iv;
  index->groups[0]->materials[14]->name = builtin_material_names[15];
  index->groups[0]->materials[14]->data = artdeco14_iv;
  index->groups[0]->materials[15]->name = builtin_material_names[16];
  index->groups[0]->materials[15]->data = artdeco15_iv;
  index->groups[0]->materials[16]->name = builtin_material_names[17];
  index->groups[0]->materials[16]->data = artdeco16_iv;
  index->groups[0]->materials[17]->name = builtin_material_names[18];
  index->groups[0]->materials[17]->data = artdeco17_iv;
  index->groups[0]->materials[18]->name = builtin_material_names[19];
  index->groups[0]->materials[18]->data = artdeco18_iv;
  index->groups[0]->materials[19]->name = builtin_material_names[20];
  index->groups[0]->materials[19]->data = artdeco19_iv;
  index->groups[0]->materials[20]->name = builtin_material_names[21];
  index->groups[0]->materials[20]->data = artdeco20_iv;
  index->groups[0]->materials[21]->name = builtin_material_names[22];
  index->groups[0]->materials[21]->data = artdeco21_iv;
  index->groups[0]->materials[22]->name = builtin_material_names[23];
  index->groups[0]->materials[22]->data = artdeco22_iv;
  index->groups[0]->materials[23]->name = builtin_material_names[24];
  index->groups[0]->materials[23]->data = artdeco23_iv;
  index->groups[0]->materials[24]->name = builtin_material_names[25];
  index->groups[0]->materials[24]->data = artdeco24_iv;
  index->groups[0]->materials[25]->name = builtin_material_names[26];
  index->groups[0]->materials[25]->data = artdeco25_iv;
  index->groups[0]->materials[26]->name = builtin_material_names[27];
  index->groups[0]->materials[26]->data = artdeco26_iv;
  index->groups[0]->materials[27]->name = builtin_material_names[28];
  index->groups[0]->materials[27]->data = artdeco27_iv;
  index->groups[0]->materials[28]->name = builtin_material_names[29];
  index->groups[0]->materials[28]->data = artdeco28_iv;
  index->groups[0]->materials[29]->name = builtin_material_names[30];
  index->groups[0]->materials[29]->data = artdeco29_iv;
  index->groups[0]->materials[30]->name = builtin_material_names[31];
  index->groups[0]->materials[30]->data = artdeco30_iv;
  index->groups[0]->materials[31]->name = builtin_material_names[32];
  index->groups[0]->materials[31]->data = artdeco31_iv;
  index->groups[0]->materials[32]->name = builtin_material_names[33];
  index->groups[0]->materials[32]->data = artdeco32_iv;
  index->groups[0]->materials[33]->name = builtin_material_names[34];
  index->groups[0]->materials[33]->data = artdeco33_iv;
  index->groups[0]->materials[34]->name = builtin_material_names[35];
  index->groups[0]->materials[34]->data = artdeco34_iv;
  index->groups[1]->name = builtin_material_names[36];
  index->groups[1]->materials[0]->name = builtin_material_names[37];
  index->groups[1]->materials[0]->data = autumn0_iv;
  index->groups[1]->materials[1]->name = builtin_material_names[38];
  index->groups[1]->materials[1]->data = autumn1_iv;
  index->groups[1]->materials[2]->name = builtin_material_names[39];
  index->groups[1]->materials[2]->data = autumn2_iv;
  index->groups[1]->materials[3]->name = builtin_material_names[40];
  index->groups[1]->materials[3]->data = autumn3_iv;
  index->groups[1]->materials[4]->name = builtin_material_names[41];
  index->groups[1]->materials[4]->data = autumn4_iv;
  index->groups[1]->materials[5]->name = builtin_material_names[42];
  index->groups[1]->materials[5]->data = autumn5_iv;
  index->groups[1]->materials[6]->name = builtin_material_names[43];
  index->groups[1]->materials[6]->data = autumn6_iv;
  index->groups[1]->materials[7]->name = builtin_material_names[44];
  index->groups[1]->materials[7]->data = autumn7_iv;
  index->groups[1]->materials[8]->name = builtin_material_names[45];
  index->groups[1]->materials[8]->data = autumn8_iv;
  index->groups[1]->materials[9]->name = builtin_material_names[46];
  index->groups[1]->materials[9]->data = autumn9_iv;
  index->groups[1]->materials[10]->name = builtin_material_names[47];
  index->groups[1]->materials[10]->data = autumn10_iv;
  index->groups[1]->materials[11]->name = builtin_material_names[48];
  index->groups[1]->materials[11]->data = autumn11_iv;
  index->groups[1]->materials[12]->name = builtin_material_names[49];
  index->groups[1]->materials[12]->data = autumn12_iv;
  index->groups[1]->materials[13]->name = builtin_material_names[50];
  index->groups[1]->materials[13]->data = autumn13_iv;
  index->groups[1]->materials[14]->name = builtin_material_names[51];
  index->groups[1]->materials[14]->data = autumn14_iv;
  index->groups[1]->materials[15]->name = builtin_material_names[52];
  index->groups[1]->materials[15]->data = autumn15_iv;
  index->groups[1]->materials[16]->name = builtin_material_names[53];
  index->groups[1]->materials[16]->data = autumn16_iv;
  index->groups[1]->materials[17]->name = builtin_material_names[54];
  index->groups[1]->materials[17]->data = autumn17_iv;
  index->groups[1]->materials[18]->name = builtin_material_names[55];
  index->groups[1]->materials[18]->data = autumn18_iv;
  index->groups[1]->materials[19]->name = builtin_material_names[56];
  index->groups[1]->materials[19]->data = autumn19_iv;
  index->groups[1]->materials[20]->name = builtin_material_names[57];
  index->groups[1]->materials[20]->data = autumn20_iv;
  index->groups[1]->materials[21]->name = builtin_material_names[58];
  index->groups[1]->materials[21]->data = autumn21_iv;
  index->groups[1]->materials[22]->name = builtin_material_names[59];
  index->groups[1]->materials[22]->data = autumn22_iv;
  index->groups[1]->materials[23]->name = builtin_material_names[60];
  index->groups[1]->materials[23]->data = autumn23_iv;
  index->groups[1]->materials[24]->name = builtin_material_names[61];
  index->groups[1]->materials[24]->data = autumn24_iv;
  index->groups[1]->materials[25]->name = builtin_material_names[62];
  index->groups[1]->materials[25]->data = autumn25_iv;
  index->groups[1]->materials[26]->name = builtin_material_names[63];
  index->groups[1]->materials[26]->data = autumn26_iv;
  index->groups[1]->materials[27]->name = builtin_material_names[64];
  index->groups[1]->materials[27]->data = autumn27_iv;
  index->groups[1]->materials[28]->name = builtin_material_names[65];
  index->groups[1]->materials[28]->data = autumn28_iv;
  index->groups[1]->materials[29]->name = builtin_material_names[66];
  index->groups[1]->materials[29]->data = autumn29_iv;
  index->groups[1]->materials[30]->name = builtin_material_names[67];
  index->groups[1]->materials[30]->data = autumn30_iv;
  index->groups[1]->materials[31]->name = builtin_material_names[68];
  index->groups[1]->materials[31]->data = autumn31_iv;
  index->groups[1]->materials[32]->name = builtin_material_names[69];
  index->groups[1]->materials[32]->data = autumn32_iv;
  index->groups[1]->materials[33]->name = builtin_material_names[70];
  index->groups[1]->materials[33]->data = autumn33_iv;
  index->groups[1]->materials[34]->name = builtin_material_names[71];
  index->groups[1]->materials[34]->data = autumn34_iv;
  index->groups[2]->name = builtin_material_names[72];
  index->groups[2]->materials[0]->name = builtin_material_names[73];
  index->groups[2]->materials[0]->data = glass0_iv;
  index->groups[2]->materials[1]->name = builtin_material_names[74];
  index->groups[2]->materials[1]->data = glass1_iv;
  index->groups[2]->materials[2]->name = builtin_material_names[75];
  index->groups[2]->materials[2]->data = glass2_iv;
  index->groups[2]->materials[3]->name = builtin_material_names[76];
  index->groups[2]->materials[3]->data = glass3_iv;
  index->groups[2]->materials[4]->name = builtin_material_names[77];
  index->groups[2]->materials[4]->data = glass4_iv;
  index->groups[2]->materials[5]->name = builtin_material_names[78];
  index->groups[2]->materials[5]->data = glass5_iv;
  index->groups[2]->materials[6]->name = builtin_material_names[79];
  index->groups[2]->materials[6]->data = glass6_iv;
  index->groups[2]->materials[7]->name = builtin_material_names[80];
  index->groups[2]->materials[7]->data = glass7_iv;
  index->groups[2]->materials[8]->name = builtin_material_names[81];
  index->groups[2]->materials[8]->data = glass8_iv;
  index->groups[2]->materials[9]->name = builtin_material_names[82];
  index->groups[2]->materials[9]->data = glass9_iv;
  index->groups[2]->materials[10]->name = builtin_material_names[83];
  index->groups[2]->materials[10]->data = glass10_iv;
  index->groups[2]->materials[11]->name = builtin_material_names[84];
  index->groups[2]->materials[11]->data = glass11_iv;
  index->groups[2]->materials[12]->name = builtin_material_names[85];
  index->groups[2]->materials[12]->data = glass12_iv;
  index->groups[2]->materials[13]->name = builtin_material_names[86];
  index->groups[2]->materials[13]->data = glass13_iv;
  index->groups[2]->materials[14]->name = builtin_material_names[87];
  index->groups[2]->materials[14]->data = glass14_iv;
  index->groups[2]->materials[15]->name = builtin_material_names[88];
  index->groups[2]->materials[15]->data = glass15_iv;
  index->groups[2]->materials[16]->name = builtin_material_names[89];
  index->groups[2]->materials[16]->data = glass16_iv;
  index->groups[2]->materials[17]->name = builtin_material_names[90];
  index->groups[2]->materials[17]->data = glass17_iv;
  index->groups[2]->materials[18]->name = builtin_material_names[91];
  index->groups[2]->materials[18]->data = glass18_iv;
  index->groups[2]->materials[19]->name = builtin_material_names[92];
  index->groups[2]->materials[19]->data = glass19_iv;
  index->groups[2]->materials[20]->name = builtin_material_names[93];
  index->groups[2]->materials[20]->data = glass20_iv;
  index->groups[2]->materials[21]->name = builtin_material_names[94];
  index->groups[2]->materials[21]->data = glass21_iv;
  index->groups[2]->materials[22]->name = builtin_material_names[95];
  index->groups[2]->materials[22]->data = glass22_iv;
  index->groups[2]->materials[23]->name = builtin_material_names[96];
  index->groups[2]->materials[23]->data = glass23_iv;
  index->groups[2]->materials[24]->name = builtin_material_names[97];
  index->groups[2]->materials[24]->data = glass24_iv;
  index->groups[2]->materials[25]->name = builtin_material_names[98];
  index->groups[2]->materials[25]->data = glass25_iv;
  index->groups[2]->materials[26]->name = builtin_material_names[99];
  index->groups[2]->materials[26]->data = glass26_iv;
  index->groups[2]->materials[27]->name = builtin_material_names[100];
  index->groups[2]->materials[27]->data = glass27_iv;
  index->groups[2]->materials[28]->name = builtin_material_names[101];
  index->groups[2]->materials[28]->data = glass28_iv;
  index->groups[2]->materials[29]->name = builtin_material_names[102];
  index->groups[2]->materials[29]->data = glass29_iv;
  index->groups[2]->materials[30]->name = builtin_material_names[103];
  index->groups[2]->materials[30]->data = glass30_iv;
  index->groups[2]->materials[31]->name = builtin_material_names[104];
  index->groups[2]->materials[31]->data = glass31_iv;
  index->groups[2]->materials[32]->name = builtin_material_names[105];
  index->groups[2]->materials[32]->data = glass32_iv;
  index->groups[2]->materials[33]->name = builtin_material_names[106];
  index->groups[2]->materials[33]->data = glass33_iv;
  index->groups[2]->materials[34]->name = builtin_material_names[107];
  index->groups[2]->materials[34]->data = glass34_iv;
  index->groups[3]->name = builtin_material_names[108];
  index->groups[3]->materials[0]->name = builtin_material_names[109];
  index->groups[3]->materials[0]->data = metal0_iv;
  index->groups[3]->materials[1]->name = builtin_material_names[110];
  index->groups[3]->materials[1]->data = metal1_iv;
  index->groups[3]->materials[2]->name = builtin_material_names[111];
  index->groups[3]->materials[2]->data = metal2_iv;
  index->groups[3]->materials[3]->name = builtin_material_names[112];
  index->groups[3]->materials[3]->data = metal3_iv;
  index->groups[3]->materials[4]->name = builtin_material_names[113];
  index->groups[3]->materials[4]->data = metal4_iv;
  index->groups[3]->materials[5]->name = builtin_material_names[114];
  index->groups[3]->materials[5]->data = metal5_iv;
  index->groups[3]->materials[6]->name = builtin_material_names[115];
  index->groups[3]->materials[6]->data = metal6_iv;
  index->groups[3]->materials[7]->name = builtin_material_names[116];
  index->groups[3]->materials[7]->data = metal7_iv;
  index->groups[3]->materials[8]->name = builtin_material_names[117];
  index->groups[3]->materials[8]->data = metal8_iv;
  index->groups[3]->materials[9]->name = builtin_material_names[118];
  index->groups[3]->materials[9]->data = metal9_iv;
  index->groups[3]->materials[10]->name = builtin_material_names[119];
  index->groups[3]->materials[10]->data = metal10_iv;
  index->groups[3]->materials[11]->name = builtin_material_names[120];
  index->groups[3]->materials[11]->data = metal11_iv;
  index->groups[3]->materials[12]->name = builtin_material_names[121];
  index->groups[3]->materials[12]->data = metal12_iv;
  index->groups[3]->materials[13]->name = builtin_material_names[122];
  index->groups[3]->materials[13]->data = metal13_iv;
  index->groups[3]->materials[14]->name = builtin_material_names[123];
  index->groups[3]->materials[14]->data = metal14_iv;
  index->groups[3]->materials[15]->name = builtin_material_names[124];
  index->groups[3]->materials[15]->data = metal15_iv;
  index->groups[3]->materials[16]->name = builtin_material_names[125];
  index->groups[3]->materials[16]->data = metal16_iv;
  index->groups[3]->materials[17]->name = builtin_material_names[126];
  index->groups[3]->materials[17]->data = metal17_iv;
  index->groups[3]->materials[18]->name = builtin_material_names[127];
  index->groups[3]->materials[18]->data = metal18_iv;
  index->groups[3]->materials[19]->name = builtin_material_names[128];
  index->groups[3]->materials[19]->data = metal19_iv;
  index->groups[3]->materials[20]->name = builtin_material_names[129];
  index->groups[3]->materials[20]->data = metal20_iv;
  index->groups[3]->materials[21]->name = builtin_material_names[130];
  index->groups[3]->materials[21]->data = metal21_iv;
  index->groups[3]->materials[22]->name = builtin_material_names[131];
  index->groups[3]->materials[22]->data = metal22_iv;
  index->groups[3]->materials[23]->name = builtin_material_names[132];
  index->groups[3]->materials[23]->data = metal23_iv;
  index->groups[3]->materials[24]->name = builtin_material_names[133];
  index->groups[3]->materials[24]->data = metal24_iv;
  index->groups[3]->materials[25]->name = builtin_material_names[134];
  index->groups[3]->materials[25]->data = metal25_iv;
  index->groups[3]->materials[26]->name = builtin_material_names[135];
  index->groups[3]->materials[26]->data = metal26_iv;
  index->groups[3]->materials[27]->name = builtin_material_names[136];
  index->groups[3]->materials[27]->data = metal27_iv;
  index->groups[3]->materials[28]->name = builtin_material_names[137];
  index->groups[3]->materials[28]->data = metal28_iv;
  index->groups[3]->materials[29]->name = builtin_material_names[138];
  index->groups[3]->materials[29]->data = metal29_iv;
  index->groups[3]->materials[30]->name = builtin_material_names[139];
  index->groups[3]->materials[30]->data = metal30_iv;
  index->groups[3]->materials[31]->name = builtin_material_names[140];
  index->groups[3]->materials[31]->data = metal31_iv;
  index->groups[3]->materials[32]->name = builtin_material_names[141];
  index->groups[3]->materials[32]->data = metal32_iv;
  index->groups[3]->materials[33]->name = builtin_material_names[142];
  index->groups[3]->materials[33]->data = metal33_iv;
  index->groups[3]->materials[34]->name = builtin_material_names[143];
  index->groups[3]->materials[34]->data = metal34_iv;
  index->groups[4]->name = builtin_material_names[144];
  index->groups[4]->materials[0]->name = builtin_material_names[145];
  index->groups[4]->materials[0]->data = neon0_iv;
  index->groups[4]->materials[1]->name = builtin_material_names[146];
  index->groups[4]->materials[1]->data = neon1_iv;
  index->groups[4]->materials[2]->name = builtin_material_names[147];
  index->groups[4]->materials[2]->data = neon2_iv;
  index->groups[4]->materials[3]->name = builtin_material_names[148];
  index->groups[4]->materials[3]->data = neon3_iv;
  index->groups[4]->materials[4]->name = builtin_material_names[149];
  index->groups[4]->materials[4]->data = neon4_iv;
  index->groups[4]->materials[5]->name = builtin_material_names[150];
  index->groups[4]->materials[5]->data = neon5_iv;
  index->groups[4]->materials[6]->name = builtin_material_names[151];
  index->groups[4]->materials[6]->data = neon6_iv;
  index->groups[4]->materials[7]->name = builtin_material_names[152];
  index->groups[4]->materials[7]->data = neon7_iv;
  index->groups[4]->materials[8]->name = builtin_material_names[153];
  index->groups[4]->materials[8]->data = neon8_iv;
  index->groups[4]->materials[9]->name = builtin_material_names[154];
  index->groups[4]->materials[9]->data = neon9_iv;
  index->groups[4]->materials[10]->name = builtin_material_names[155];
  index->groups[4]->materials[10]->data = neon10_iv;
  index->groups[4]->materials[11]->name = builtin_material_names[156];
  index->groups[4]->materials[11]->data = neon11_iv;
  index->groups[4]->materials[12]->name = builtin_material_names[157];
  index->groups[4]->materials[12]->data = neon12_iv;
  index->groups[4]->materials[13]->name = builtin_material_names[158];
  index->groups[4]->materials[13]->data = neon13_iv;
  index->groups[4]->materials[14]->name = builtin_material_names[159];
  index->groups[4]->materials[14]->data = neon14_iv;
  index->groups[4]->materials[15]->name = builtin_material_names[160];
  index->groups[4]->materials[15]->data = neon15_iv;
  index->groups[4]->materials[16]->name = builtin_material_names[161];
  index->groups[4]->materials[16]->data = neon16_iv;
  index->groups[4]->materials[17]->name = builtin_material_names[162];
  index->groups[4]->materials[17]->data = neon17_iv;
  index->groups[4]->materials[18]->name = builtin_material_names[163];
  index->groups[4]->materials[18]->data = neon18_iv;
  index->groups[4]->materials[19]->name = builtin_material_names[164];
  index->groups[4]->materials[19]->data = neon19_iv;
  index->groups[4]->materials[20]->name = builtin_material_names[165];
  index->groups[4]->materials[20]->data = neon20_iv;
  index->groups[4]->materials[21]->name = builtin_material_names[166];
  index->groups[4]->materials[21]->data = neon21_iv;
  index->groups[4]->materials[22]->name = builtin_material_names[167];
  index->groups[4]->materials[22]->data = neon22_iv;
  index->groups[4]->materials[23]->name = builtin_material_names[168];
  index->groups[4]->materials[23]->data = neon23_iv;
  index->groups[4]->materials[24]->name = builtin_material_names[169];
  index->groups[4]->materials[24]->data = neon24_iv;
  index->groups[4]->materials[25]->name = builtin_material_names[170];
  index->groups[4]->materials[25]->data = neon25_iv;
  index->groups[4]->materials[26]->name = builtin_material_names[171];
  index->groups[4]->materials[26]->data = neon26_iv;
  index->groups[4]->materials[27]->name = builtin_material_names[172];
  index->groups[4]->materials[27]->data = neon27_iv;
  index->groups[4]->materials[28]->name = builtin_material_names[173];
  index->groups[4]->materials[28]->data = neon28_iv;
  index->groups[4]->materials[29]->name = builtin_material_names[174];
  index->groups[4]->materials[29]->data = neon29_iv;
  index->groups[4]->materials[30]->name = builtin_material_names[175];
  index->groups[4]->materials[30]->data = neon30_iv;
  index->groups[4]->materials[31]->name = builtin_material_names[176];
  index->groups[4]->materials[31]->data = neon31_iv;
  index->groups[4]->materials[32]->name = builtin_material_names[177];
  index->groups[4]->materials[32]->data = neon32_iv;
  index->groups[4]->materials[33]->name = builtin_material_names[178];
  index->groups[4]->materials[33]->data = neon33_iv;
  index->groups[4]->materials[34]->name = builtin_material_names[179];
  index->groups[4]->materials[34]->data = neon34_iv;
  index->groups[5]->name = builtin_material_names[180];
  index->groups[5]->materials[0]->name = builtin_material_names[181];
  index->groups[5]->materials[0]->data = rococo0_iv;
  index->groups[5]->materials[1]->name = builtin_material_names[182];
  index->groups[5]->materials[1]->data = rococo1_iv;
  index->groups[5]->materials[2]->name = builtin_material_names[183];
  index->groups[5]->materials[2]->data = rococo2_iv;
  index->groups[5]->materials[3]->name = builtin_material_names[184];
  index->groups[5]->materials[3]->data = rococo3_iv;
  index->groups[5]->materials[4]->name = builtin_material_names[185];
  index->groups[5]->materials[4]->data = rococo4_iv;
  index->groups[5]->materials[5]->name = builtin_material_names[186];
  index->groups[5]->materials[5]->data = rococo5_iv;
  index->groups[5]->materials[6]->name = builtin_material_names[187];
  index->groups[5]->materials[6]->data = rococo6_iv;
  index->groups[5]->materials[7]->name = builtin_material_names[188];
  index->groups[5]->materials[7]->data = rococo7_iv;
  index->groups[5]->materials[8]->name = builtin_material_names[189];
  index->groups[5]->materials[8]->data = rococo8_iv;
  index->groups[5]->materials[9]->name = builtin_material_names[190];
  index->groups[5]->materials[9]->data = rococo9_iv;
  index->groups[5]->materials[10]->name = builtin_material_names[191];
  index->groups[5]->materials[10]->data = rococo10_iv;
  index->groups[5]->materials[11]->name = builtin_material_names[192];
  index->groups[5]->materials[11]->data = rococo11_iv;
  index->groups[5]->materials[12]->name = builtin_material_names[193];
  index->groups[5]->materials[12]->data = rococo12_iv;
  index->groups[5]->materials[13]->name = builtin_material_names[194];
  index->groups[5]->materials[13]->data = rococo13_iv;
  index->groups[5]->materials[14]->name = builtin_material_names[195];
  index->groups[5]->materials[14]->data = rococo14_iv;
  index->groups[5]->materials[15]->name = builtin_material_names[196];
  index->groups[5]->materials[15]->data = rococo15_iv;
  index->groups[5]->materials[16]->name = builtin_material_names[197];
  index->groups[5]->materials[16]->data = rococo16_iv;
  index->groups[5]->materials[17]->name = builtin_material_names[198];
  index->groups[5]->materials[17]->data = rococo17_iv;
  index->groups[5]->materials[18]->name = builtin_material_names[199];
  index->groups[5]->materials[18]->data = rococo18_iv;
  index->groups[5]->materials[19]->name = builtin_material_names[200];
  index->groups[5]->materials[19]->data = rococo19_iv;
  index->groups[5]->materials[20]->name = builtin_material_names[201];
  index->groups[5]->materials[20]->data = rococo20_iv;
  index->groups[5]->materials[21]->name = builtin_material_names[202];
  index->groups[5]->materials[21]->data = rococo21_iv;
  index->groups[5]->materials[22]->name = builtin_material_names[203];
  index->groups[5]->materials[22]->data = rococo22_iv;
  index->groups[5]->materials[23]->name = builtin_material_names[204];
  index->groups[5]->materials[23]->data = rococo23_iv;
  index->groups[5]->materials[24]->name = builtin_material_names[205];
  index->groups[5]->materials[24]->data = rococo24_iv;
  index->groups[5]->materials[25]->name = builtin_material_names[206];
  index->groups[5]->materials[25]->data = rococo25_iv;
  index->groups[5]->materials[26]->name = builtin_material_names[207];
  index->groups[5]->materials[26]->data = rococo26_iv;
  index->groups[5]->materials[27]->name = builtin_material_names[208];
  index->groups[5]->materials[27]->data = rococo27_iv;
  index->groups[5]->materials[28]->name = builtin_material_names[209];
  index->groups[5]->materials[28]->data = rococo28_iv;
  index->groups[5]->materials[29]->name = builtin_material_names[210];
  index->groups[5]->materials[29]->data = rococo29_iv;
  index->groups[5]->materials[30]->name = builtin_material_names[211];
  index->groups[5]->materials[30]->data = rococo30_iv;
  index->groups[5]->materials[31]->name = builtin_material_names[212];
  index->groups[5]->materials[31]->data = rococo31_iv;
  index->groups[5]->materials[32]->name = builtin_material_names[213];
  index->groups[5]->materials[32]->data = rococo32_iv;
  index->groups[5]->materials[33]->name = builtin_material_names[214];
  index->groups[5]->materials[33]->data = rococo33_iv;
  index->groups[5]->materials[34]->name = builtin_material_names[215];
  index->groups[5]->materials[34]->data = rococo34_iv;
  index->groups[6]->name = builtin_material_names[216];
  index->groups[6]->materials[0]->name = builtin_material_names[217];
  index->groups[6]->materials[0]->data = santafe0_iv;
  index->groups[6]->materials[1]->name = builtin_material_names[218];
  index->groups[6]->materials[1]->data = santafe1_iv;
  index->groups[6]->materials[2]->name = builtin_material_names[219];
  index->groups[6]->materials[2]->data = santafe2_iv;
  index->groups[6]->materials[3]->name = builtin_material_names[220];
  index->groups[6]->materials[3]->data = santafe3_iv;
  index->groups[6]->materials[4]->name = builtin_material_names[221];
  index->groups[6]->materials[4]->data = santafe4_iv;
  index->groups[6]->materials[5]->name = builtin_material_names[222];
  index->groups[6]->materials[5]->data = santafe5_iv;
  index->groups[6]->materials[6]->name = builtin_material_names[223];
  index->groups[6]->materials[6]->data = santafe6_iv;
  index->groups[6]->materials[7]->name = builtin_material_names[224];
  index->groups[6]->materials[7]->data = santafe7_iv;
  index->groups[6]->materials[8]->name = builtin_material_names[225];
  index->groups[6]->materials[8]->data = santafe8_iv;
  index->groups[6]->materials[9]->name = builtin_material_names[226];
  index->groups[6]->materials[9]->data = santafe9_iv;
  index->groups[6]->materials[10]->name = builtin_material_names[227];
  index->groups[6]->materials[10]->data = santafe10_iv;
  index->groups[6]->materials[11]->name = builtin_material_names[228];
  index->groups[6]->materials[11]->data = santafe11_iv;
  index->groups[6]->materials[12]->name = builtin_material_names[229];
  index->groups[6]->materials[12]->data = santafe12_iv;
  index->groups[6]->materials[13]->name = builtin_material_names[230];
  index->groups[6]->materials[13]->data = santafe13_iv;
  index->groups[6]->materials[14]->name = builtin_material_names[231];
  index->groups[6]->materials[14]->data = santafe14_iv;
  index->groups[6]->materials[15]->name = builtin_material_names[232];
  index->groups[6]->materials[15]->data = santafe15_iv;
  index->groups[6]->materials[16]->name = builtin_material_names[233];
  index->groups[6]->materials[16]->data = santafe16_iv;
  index->groups[6]->materials[17]->name = builtin_material_names[234];
  index->groups[6]->materials[17]->data = santafe17_iv;
  index->groups[6]->materials[18]->name = builtin_material_names[235];
  index->groups[6]->materials[18]->data = santafe18_iv;
  index->groups[6]->materials[19]->name = builtin_material_names[236];
  index->groups[6]->materials[19]->data = santafe19_iv;
  index->groups[6]->materials[20]->name = builtin_material_names[237];
  index->groups[6]->materials[20]->data = santafe20_iv;
  index->groups[6]->materials[21]->name = builtin_material_names[238];
  index->groups[6]->materials[21]->data = santafe21_iv;
  index->groups[6]->materials[22]->name = builtin_material_names[239];
  index->groups[6]->materials[22]->data = santafe22_iv;
  index->groups[6]->materials[23]->name = builtin_material_names[240];
  index->groups[6]->materials[23]->data = santafe23_iv;
  index->groups[6]->materials[24]->name = builtin_material_names[241];
  index->groups[6]->materials[24]->data = santafe24_iv;
  index->groups[6]->materials[25]->name = builtin_material_names[242];
  index->groups[6]->materials[25]->data = santafe25_iv;
  index->groups[6]->materials[26]->name = builtin_material_names[243];
  index->groups[6]->materials[26]->data = santafe26_iv;
  index->groups[6]->materials[27]->name = builtin_material_names[244];
  index->groups[6]->materials[27]->data = santafe27_iv;
  index->groups[6]->materials[28]->name = builtin_material_names[245];
  index->groups[6]->materials[28]->data = santafe28_iv;
  index->groups[6]->materials[29]->name = builtin_material_names[246];
  index->groups[6]->materials[29]->data = santafe29_iv;
  index->groups[6]->materials[30]->name = builtin_material_names[247];
  index->groups[6]->materials[30]->data = santafe30_iv;
  index->groups[6]->materials[31]->name = builtin_material_names[248];
  index->groups[6]->materials[31]->data = santafe31_iv;
  index->groups[6]->materials[32]->name = builtin_material_names[249];
  index->groups[6]->materials[32]->data = santafe32_iv;
  index->groups[6]->materials[33]->name = builtin_material_names[250];
  index->groups[6]->materials[33]->data = santafe33_iv;
  index->groups[6]->materials[34]->name = builtin_material_names[251];
  index->groups[6]->materials[34]->data = santafe34_iv;
  index->groups[7]->name = builtin_material_names[252];
  index->groups[7]->materials[0]->name = builtin_material_names[253];
  index->groups[7]->materials[0]->data = sheen0_iv;
  index->groups[7]->materials[1]->name = builtin_material_names[254];
  index->groups[7]->materials[1]->data = sheen1_iv;
  index->groups[7]->materials[2]->name = builtin_material_names[255];
  index->groups[7]->materials[2]->data = sheen2_iv;
  index->groups[7]->materials[3]->name = builtin_material_names[256];
  index->groups[7]->materials[3]->data = sheen3_iv;
  index->groups[7]->materials[4]->name = builtin_material_names[257];
  index->groups[7]->materials[4]->data = sheen4_iv;
  index->groups[7]->materials[5]->name = builtin_material_names[258];
  index->groups[7]->materials[5]->data = sheen5_iv;
  index->groups[7]->materials[6]->name = builtin_material_names[259];
  index->groups[7]->materials[6]->data = sheen6_iv;
  index->groups[7]->materials[7]->name = builtin_material_names[260];
  index->groups[7]->materials[7]->data = sheen7_iv;
  index->groups[7]->materials[8]->name = builtin_material_names[261];
  index->groups[7]->materials[8]->data = sheen8_iv;
  index->groups[7]->materials[9]->name = builtin_material_names[262];
  index->groups[7]->materials[9]->data = sheen9_iv;
  index->groups[7]->materials[10]->name = builtin_material_names[263];
  index->groups[7]->materials[10]->data = sheen10_iv;
  index->groups[7]->materials[11]->name = builtin_material_names[264];
  index->groups[7]->materials[11]->data = sheen11_iv;
  index->groups[7]->materials[12]->name = builtin_material_names[265];
  index->groups[7]->materials[12]->data = sheen12_iv;
  index->groups[7]->materials[13]->name = builtin_material_names[266];
  index->groups[7]->materials[13]->data = sheen13_iv;
  index->groups[7]->materials[14]->name = builtin_material_names[267];
  index->groups[7]->materials[14]->data = sheen14_iv;
  index->groups[7]->materials[15]->name = builtin_material_names[268];
  index->groups[7]->materials[15]->data = sheen15_iv;
  index->groups[7]->materials[16]->name = builtin_material_names[269];
  index->groups[7]->materials[16]->data = sheen16_iv;
  index->groups[7]->materials[17]->name = builtin_material_names[270];
  index->groups[7]->materials[17]->data = sheen17_iv;
  index->groups[7]->materials[18]->name = builtin_material_names[271];
  index->groups[7]->materials[18]->data = sheen18_iv;
  index->groups[7]->materials[19]->name = builtin_material_names[272];
  index->groups[7]->materials[19]->data = sheen19_iv;
  index->groups[7]->materials[20]->name = builtin_material_names[273];
  index->groups[7]->materials[20]->data = sheen20_iv;
  index->groups[7]->materials[21]->name = builtin_material_names[274];
  index->groups[7]->materials[21]->data = sheen21_iv;
  index->groups[7]->materials[22]->name = builtin_material_names[275];
  index->groups[7]->materials[22]->data = sheen22_iv;
  index->groups[7]->materials[23]->name = builtin_material_names[276];
  index->groups[7]->materials[23]->data = sheen23_iv;
  index->groups[7]->materials[24]->name = builtin_material_names[277];
  index->groups[7]->materials[24]->data = sheen24_iv;
  index->groups[7]->materials[25]->name = builtin_material_names[278];
  index->groups[7]->materials[25]->data = sheen25_iv;
  index->groups[7]->materials[26]->name = builtin_material_names[279];
  index->groups[7]->materials[26]->data = sheen26_iv;
  index->groups[7]->materials[27]->name = builtin_material_names[280];
  index->groups[7]->materials[27]->data = sheen27_iv;
  index->groups[7]->materials[28]->name = builtin_material_names[281];
  index->groups[7]->materials[28]->data = sheen28_iv;
  index->groups[7]->materials[29]->name = builtin_material_names[282];
  index->groups[7]->materials[29]->data = sheen29_iv;
  index->groups[7]->materials[30]->name = builtin_material_names[283];
  index->groups[7]->materials[30]->data = sheen30_iv;
  index->groups[7]->materials[31]->name = builtin_material_names[284];
  index->groups[7]->materials[31]->data = sheen31_iv;
  index->groups[7]->materials[32]->name = builtin_material_names[285];
  index->groups[7]->materials[32]->data = sheen32_iv;
  index->groups[7]->materials[33]->name = builtin_material_names[286];
  index->groups[7]->materials[33]->data = sheen33_iv;
  index->groups[7]->materials[34]->name = builtin_material_names[287];
  index->groups[7]->materials[34]->data = sheen34_iv;
  index->groups[8]->name = builtin_material_names[288];
  index->groups[8]->materials[0]->name = builtin_material_names[289];
  index->groups[8]->materials[0]->data = silky0_iv;
  index->groups[8]->materials[1]->name = builtin_material_names[290];
  index->groups[8]->materials[1]->data = silky1_iv;
  index->groups[8]->materials[2]->name = builtin_material_names[291];
  index->groups[8]->materials[2]->data = silky2_iv;
  index->groups[8]->materials[3]->name = builtin_material_names[292];
  index->groups[8]->materials[3]->data = silky3_iv;
  index->groups[8]->materials[4]->name = builtin_material_names[293];
  index->groups[8]->materials[4]->data = silky4_iv;
  index->groups[8]->materials[5]->name = builtin_material_names[294];
  index->groups[8]->materials[5]->data = silky5_iv;
  index->groups[8]->materials[6]->name = builtin_material_names[295];
  index->groups[8]->materials[6]->data = silky6_iv;
  index->groups[8]->materials[7]->name = builtin_material_names[296];
  index->groups[8]->materials[7]->data = silky7_iv;
  index->groups[8]->materials[8]->name = builtin_material_names[297];
  index->groups[8]->materials[8]->data = silky8_iv;
  index->groups[8]->materials[9]->name = builtin_material_names[298];
  index->groups[8]->materials[9]->data = silky9_iv;
  index->groups[8]->materials[10]->name = builtin_material_names[299];
  index->groups[8]->materials[10]->data = silky10_iv;
  index->groups[8]->materials[11]->name = builtin_material_names[300];
  index->groups[8]->materials[11]->data = silky11_iv;
  index->groups[8]->materials[12]->name = builtin_material_names[301];
  index->groups[8]->materials[12]->data = silky12_iv;
  index->groups[8]->materials[13]->name = builtin_material_names[302];
  index->groups[8]->materials[13]->data = silky13_iv;
  index->groups[8]->materials[14]->name = builtin_material_names[303];
  index->groups[8]->materials[14]->data = silky14_iv;
  index->groups[8]->materials[15]->name = builtin_material_names[304];
  index->groups[8]->materials[15]->data = silky15_iv;
  index->groups[8]->materials[16]->name = builtin_material_names[305];
  index->groups[8]->materials[16]->data = silky16_iv;
  index->groups[8]->materials[17]->name = builtin_material_names[306];
  index->groups[8]->materials[17]->data = silky17_iv;
  index->groups[8]->materials[18]->name = builtin_material_names[307];
  index->groups[8]->materials[18]->data = silky18_iv;
  index->groups[8]->materials[19]->name = builtin_material_names[308];
  index->groups[8]->materials[19]->data = silky19_iv;
  index->groups[8]->materials[20]->name = builtin_material_names[309];
  index->groups[8]->materials[20]->data = silky20_iv;
  index->groups[8]->materials[21]->name = builtin_material_names[310];
  index->groups[8]->materials[21]->data = silky21_iv;
  index->groups[8]->materials[22]->name = builtin_material_names[311];
  index->groups[8]->materials[22]->data = silky22_iv;
  index->groups[8]->materials[23]->name = builtin_material_names[312];
  index->groups[8]->materials[23]->data = silky23_iv;
  index->groups[8]->materials[24]->name = builtin_material_names[313];
  index->groups[8]->materials[24]->data = silky24_iv;
  index->groups[8]->materials[25]->name = builtin_material_names[314];
  index->groups[8]->materials[25]->data = silky25_iv;
  index->groups[8]->materials[26]->name = builtin_material_names[315];
  index->groups[8]->materials[26]->data = silky26_iv;
  index->groups[8]->materials[27]->name = builtin_material_names[316];
  index->groups[8]->materials[27]->data = silky27_iv;
  index->groups[8]->materials[28]->name = builtin_material_names[317];
  index->groups[8]->materials[28]->data = silky28_iv;
  index->groups[8]->materials[29]->name = builtin_material_names[318];
  index->groups[8]->materials[29]->data = silky29_iv;
  index->groups[8]->materials[30]->name = builtin_material_names[319];
  index->groups[8]->materials[30]->data = silky30_iv;
  index->groups[8]->materials[31]->name = builtin_material_names[320];
  index->groups[8]->materials[31]->data = silky31_iv;
  index->groups[8]->materials[32]->name = builtin_material_names[321];
  index->groups[8]->materials[32]->data = silky32_iv;
  index->groups[8]->materials[33]->name = builtin_material_names[322];
  index->groups[8]->materials[33]->data = silky33_iv;
  index->groups[8]->materials[34]->name = builtin_material_names[323];
  index->groups[8]->materials[34]->data = silky34_iv;
  index->groups[9]->name = builtin_material_names[324];
  index->groups[9]->materials[0]->name = builtin_material_names[325];
  index->groups[9]->materials[0]->data = spring0_iv;
  index->groups[9]->materials[1]->name = builtin_material_names[326];
  index->groups[9]->materials[1]->data = spring1_iv;
  index->groups[9]->materials[2]->name = builtin_material_names[327];
  index->groups[9]->materials[2]->data = spring2_iv;
  index->groups[9]->materials[3]->name = builtin_material_names[328];
  index->groups[9]->materials[3]->data = spring3_iv;
  index->groups[9]->materials[4]->name = builtin_material_names[329];
  index->groups[9]->materials[4]->data = spring4_iv;
  index->groups[9]->materials[5]->name = builtin_material_names[330];
  index->groups[9]->materials[5]->data = spring5_iv;
  index->groups[9]->materials[6]->name = builtin_material_names[331];
  index->groups[9]->materials[6]->data = spring6_iv;
  index->groups[9]->materials[7]->name = builtin_material_names[332];
  index->groups[9]->materials[7]->data = spring7_iv;
  index->groups[9]->materials[8]->name = builtin_material_names[333];
  index->groups[9]->materials[8]->data = spring8_iv;
  index->groups[9]->materials[9]->name = builtin_material_names[334];
  index->groups[9]->materials[9]->data = spring9_iv;
  index->groups[9]->materials[10]->name = builtin_material_names[335];
  index->groups[9]->materials[10]->data = spring10_iv;
  index->groups[9]->materials[11]->name = builtin_material_names[336];
  index->groups[9]->materials[11]->data = spring11_iv;
  index->groups[9]->materials[12]->name = builtin_material_names[337];
  index->groups[9]->materials[12]->data = spring12_iv;
  index->groups[9]->materials[13]->name = builtin_material_names[338];
  index->groups[9]->materials[13]->data = spring13_iv;
  index->groups[9]->materials[14]->name = builtin_material_names[339];
  index->groups[9]->materials[14]->data = spring14_iv;
  index->groups[9]->materials[15]->name = builtin_material_names[340];
  index->groups[9]->materials[15]->data = spring15_iv;
  index->groups[9]->materials[16]->name = builtin_material_names[341];
  index->groups[9]->materials[16]->data = spring16_iv;
  index->groups[9]->materials[17]->name = builtin_material_names[342];
  index->groups[9]->materials[17]->data = spring17_iv;
  index->groups[9]->materials[18]->name = builtin_material_names[343];
  index->groups[9]->materials[18]->data = spring18_iv;
  index->groups[9]->materials[19]->name = builtin_material_names[344];
  index->groups[9]->materials[19]->data = spring19_iv;
  index->groups[9]->materials[20]->name = builtin_material_names[345];
  index->groups[9]->materials[20]->data = spring20_iv;
  index->groups[9]->materials[21]->name = builtin_material_names[346];
  index->groups[9]->materials[21]->data = spring21_iv;
  index->groups[9]->materials[22]->name = builtin_material_names[347];
  index->groups[9]->materials[22]->data = spring22_iv;
  index->groups[9]->materials[23]->name = builtin_material_names[348];
  index->groups[9]->materials[23]->data = spring23_iv;
  index->groups[9]->materials[24]->name = builtin_material_names[349];
  index->groups[9]->materials[24]->data = spring24_iv;
  index->groups[9]->materials[25]->name = builtin_material_names[350];
  index->groups[9]->materials[25]->data = spring25_iv;
  index->groups[9]->materials[26]->name = builtin_material_names[351];
  index->groups[9]->materials[26]->data = spring26_iv;
  index->groups[9]->materials[27]->name = builtin_material_names[352];
  index->groups[9]->materials[27]->data = spring27_iv;
  index->groups[9]->materials[28]->name = builtin_material_names[353];
  index->groups[9]->materials[28]->data = spring28_iv;
  index->groups[9]->materials[29]->name = builtin_material_names[354];
  index->groups[9]->materials[29]->data = spring29_iv;
  index->groups[9]->materials[30]->name = builtin_material_names[355];
  index->groups[9]->materials[30]->data = spring30_iv;
  index->groups[9]->materials[31]->name = builtin_material_names[356];
  index->groups[9]->materials[31]->data = spring31_iv;
  index->groups[9]->materials[32]->name = builtin_material_names[357];
  index->groups[9]->materials[32]->data = spring32_iv;
  index->groups[9]->materials[33]->name = builtin_material_names[358];
  index->groups[9]->materials[33]->data = spring33_iv;
  index->groups[9]->materials[34]->name = builtin_material_names[359];
  index->groups[9]->materials[34]->data = spring34_iv;
  index->groups[10]->name = builtin_material_names[360];
  index->groups[10]->materials[0]->name = builtin_material_names[361];
  index->groups[10]->materials[0]->data = summer0_iv;
  index->groups[10]->materials[1]->name = builtin_material_names[362];
  index->groups[10]->materials[1]->data = summer1_iv;
  index->groups[10]->materials[2]->name = builtin_material_names[363];
  index->groups[10]->materials[2]->data = summer2_iv;
  index->groups[10]->materials[3]->name = builtin_material_names[364];
  index->groups[10]->materials[3]->data = summer3_iv;
  index->groups[10]->materials[4]->name = builtin_material_names[365];
  index->groups[10]->materials[4]->data = summer4_iv;
  index->groups[10]->materials[5]->name = builtin_material_names[366];
  index->groups[10]->materials[5]->data = summer5_iv;
  index->groups[10]->materials[6]->name = builtin_material_names[367];
  index->groups[10]->materials[6]->data = summer6_iv;
  index->groups[10]->materials[7]->name = builtin_material_names[368];
  index->groups[10]->materials[7]->data = summer7_iv;
  index->groups[10]->materials[8]->name = builtin_material_names[369];
  index->groups[10]->materials[8]->data = summer8_iv;
  index->groups[10]->materials[9]->name = builtin_material_names[370];
  index->groups[10]->materials[9]->data = summer9_iv;
  index->groups[10]->materials[10]->name = builtin_material_names[371];
  index->groups[10]->materials[10]->data = summer10_iv;
  index->groups[10]->materials[11]->name = builtin_material_names[372];
  index->groups[10]->materials[11]->data = summer11_iv;
  index->groups[10]->materials[12]->name = builtin_material_names[373];
  index->groups[10]->materials[12]->data = summer12_iv;
  index->groups[10]->materials[13]->name = builtin_material_names[374];
  index->groups[10]->materials[13]->data = summer13_iv;
  index->groups[10]->materials[14]->name = builtin_material_names[375];
  index->groups[10]->materials[14]->data = summer14_iv;
  index->groups[10]->materials[15]->name = builtin_material_names[376];
  index->groups[10]->materials[15]->data = summer15_iv;
  index->groups[10]->materials[16]->name = builtin_material_names[377];
  index->groups[10]->materials[16]->data = summer16_iv;
  index->groups[10]->materials[17]->name = builtin_material_names[378];
  index->groups[10]->materials[17]->data = summer17_iv;
  index->groups[10]->materials[18]->name = builtin_material_names[379];
  index->groups[10]->materials[18]->data = summer18_iv;
  index->groups[10]->materials[19]->name = builtin_material_names[380];
  index->groups[10]->materials[19]->data = summer19_iv;
  index->groups[10]->materials[20]->name = builtin_material_names[381];
  index->groups[10]->materials[20]->data = summer20_iv;
  index->groups[10]->materials[21]->name = builtin_material_names[382];
  index->groups[10]->materials[21]->data = summer21_iv;
  index->groups[10]->materials[22]->name = builtin_material_names[383];
  index->groups[10]->materials[22]->data = summer22_iv;
  index->groups[10]->materials[23]->name = builtin_material_names[384];
  index->groups[10]->materials[23]->data = summer23_iv;
  index->groups[10]->materials[24]->name = builtin_material_names[385];
  index->groups[10]->materials[24]->data = summer24_iv;
  index->groups[10]->materials[25]->name = builtin_material_names[386];
  index->groups[10]->materials[25]->data = summer25_iv;
  index->groups[10]->materials[26]->name = builtin_material_names[387];
  index->groups[10]->materials[26]->data = summer26_iv;
  index->groups[10]->materials[27]->name = builtin_material_names[388];
  index->groups[10]->materials[27]->data = summer27_iv;
  index->groups[10]->materials[28]->name = builtin_material_names[389];
  index->groups[10]->materials[28]->data = summer28_iv;
  index->groups[10]->materials[29]->name = builtin_material_names[390];
  index->groups[10]->materials[29]->data = summer29_iv;
  index->groups[10]->materials[30]->name = builtin_material_names[391];
  index->groups[10]->materials[30]->data = summer30_iv;
  index->groups[10]->materials[31]->name = builtin_material_names[392];
  index->groups[10]->materials[31]->data = summer31_iv;
  index->groups[10]->materials[32]->name = builtin_material_names[393];
  index->groups[10]->materials[32]->data = summer32_iv;
  index->groups[10]->materials[33]->name = builtin_material_names[394];
  index->groups[10]->materials[33]->data = summer33_iv;
  index->groups[10]->materials[34]->name = builtin_material_names[395];
  index->groups[10]->materials[34]->data = summer34_iv;
  index->groups[11]->name = builtin_material_names[396];
  index->groups[11]->materials[0]->name = builtin_material_names[397];
  index->groups[11]->materials[0]->data = tropical0_iv;
  index->groups[11]->materials[1]->name = builtin_material_names[398];
  index->groups[11]->materials[1]->data = tropical1_iv;
  index->groups[11]->materials[2]->name = builtin_material_names[399];
  index->groups[11]->materials[2]->data = tropical2_iv;
  index->groups[11]->materials[3]->name = builtin_material_names[400];
  index->groups[11]->materials[3]->data = tropical3_iv;
  index->groups[11]->materials[4]->name = builtin_material_names[401];
  index->groups[11]->materials[4]->data = tropical4_iv;
  index->groups[11]->materials[5]->name = builtin_material_names[402];
  index->groups[11]->materials[5]->data = tropical5_iv;
  index->groups[11]->materials[6]->name = builtin_material_names[403];
  index->groups[11]->materials[6]->data = tropical6_iv;
  index->groups[11]->materials[7]->name = builtin_material_names[404];
  index->groups[11]->materials[7]->data = tropical7_iv;
  index->groups[11]->materials[8]->name = builtin_material_names[405];
  index->groups[11]->materials[8]->data = tropical8_iv;
  index->groups[11]->materials[9]->name = builtin_material_names[406];
  index->groups[11]->materials[9]->data = tropical9_iv;
  index->groups[11]->materials[10]->name = builtin_material_names[407];
  index->groups[11]->materials[10]->data = tropical10_iv;
  index->groups[11]->materials[11]->name = builtin_material_names[408];
  index->groups[11]->materials[11]->data = tropical11_iv;
  index->groups[11]->materials[12]->name = builtin_material_names[409];
  index->groups[11]->materials[12]->data = tropical12_iv;
  index->groups[11]->materials[13]->name = builtin_material_names[410];
  index->groups[11]->materials[13]->data = tropical13_iv;
  index->groups[11]->materials[14]->name = builtin_material_names[411];
  index->groups[11]->materials[14]->data = tropical14_iv;
  index->groups[11]->materials[15]->name = builtin_material_names[412];
  index->groups[11]->materials[15]->data = tropical15_iv;
  index->groups[11]->materials[16]->name = builtin_material_names[413];
  index->groups[11]->materials[16]->data = tropical16_iv;
  index->groups[11]->materials[17]->name = builtin_material_names[414];
  index->groups[11]->materials[17]->data = tropical17_iv;
  index->groups[11]->materials[18]->name = builtin_material_names[415];
  index->groups[11]->materials[18]->data = tropical18_iv;
  index->groups[11]->materials[19]->name = builtin_material_names[416];
  index->groups[11]->materials[19]->data = tropical19_iv;
  index->groups[11]->materials[20]->name = builtin_material_names[417];
  index->groups[11]->materials[20]->data = tropical20_iv;
  index->groups[11]->materials[21]->name = builtin_material_names[418];
  index->groups[11]->materials[21]->data = tropical21_iv;
  index->groups[11]->materials[22]->name = builtin_material_names[419];
  index->groups[11]->materials[22]->data = tropical22_iv;
  index->groups[11]->materials[23]->name = builtin_material_names[420];
  index->groups[11]->materials[23]->data = tropical23_iv;
  index->groups[11]->materials[24]->name = builtin_material_names[421];
  index->groups[11]->materials[24]->data = tropical24_iv;
  index->groups[11]->materials[25]->name = builtin_material_names[422];
  index->groups[11]->materials[25]->data = tropical25_iv;
  index->groups[11]->materials[26]->name = builtin_material_names[423];
  index->groups[11]->materials[26]->data = tropical26_iv;
  index->groups[11]->materials[27]->name = builtin_material_names[424];
  index->groups[11]->materials[27]->data = tropical27_iv;
  index->groups[11]->materials[28]->name = builtin_material_names[425];
  index->groups[11]->materials[28]->data = tropical28_iv;
  index->groups[11]->materials[29]->name = builtin_material_names[426];
  index->groups[11]->materials[29]->data = tropical29_iv;
  index->groups[11]->materials[30]->name = builtin_material_names[427];
  index->groups[11]->materials[30]->data = tropical30_iv;
  index->groups[11]->materials[31]->name = builtin_material_names[428];
  index->groups[11]->materials[31]->data = tropical31_iv;
  index->groups[11]->materials[32]->name = builtin_material_names[429];
  index->groups[11]->materials[32]->data = tropical32_iv;
  index->groups[11]->materials[33]->name = builtin_material_names[430];
  index->groups[11]->materials[33]->data = tropical33_iv;
  index->groups[11]->materials[34]->name = builtin_material_names[431];
  index->groups[11]->materials[34]->data = tropical34_iv;
  index->groups[12]->name = builtin_material_names[432];
  index->groups[12]->materials[0]->name = builtin_material_names[433];
  index->groups[12]->materials[0]->data = winter0_iv;
  index->groups[12]->materials[1]->name = builtin_material_names[434];
  index->groups[12]->materials[1]->data = winter1_iv;
  index->groups[12]->materials[2]->name = builtin_material_names[435];
  index->groups[12]->materials[2]->data = winter2_iv;
  index->groups[12]->materials[3]->name = builtin_material_names[436];
  index->groups[12]->materials[3]->data = winter3_iv;
  index->groups[12]->materials[4]->name = builtin_material_names[437];
  index->groups[12]->materials[4]->data = winter4_iv;
  index->groups[12]->materials[5]->name = builtin_material_names[438];
  index->groups[12]->materials[5]->data = winter5_iv;
  index->groups[12]->materials[6]->name = builtin_material_names[439];
  index->groups[12]->materials[6]->data = winter6_iv;
  index->groups[12]->materials[7]->name = builtin_material_names[440];
  index->groups[12]->materials[7]->data = winter7_iv;
  index->groups[12]->materials[8]->name = builtin_material_names[441];
  index->groups[12]->materials[8]->data = winter8_iv;
  index->groups[12]->materials[9]->name = builtin_material_names[442];
  index->groups[12]->materials[9]->data = winter9_iv;
  index->groups[12]->materials[10]->name = builtin_material_names[443];
  index->groups[12]->materials[10]->data = winter10_iv;
  index->groups[12]->materials[11]->name = builtin_material_names[444];
  index->groups[12]->materials[11]->data = winter11_iv;
  index->groups[12]->materials[12]->name = builtin_material_names[445];
  index->groups[12]->materials[12]->data = winter12_iv;
  index->groups[12]->materials[13]->name = builtin_material_names[446];
  index->groups[12]->materials[13]->data = winter13_iv;
  index->groups[12]->materials[14]->name = builtin_material_names[447];
  index->groups[12]->materials[14]->data = winter14_iv;
  index->groups[12]->materials[15]->name = builtin_material_names[448];
  index->groups[12]->materials[15]->data = winter15_iv;
  index->groups[12]->materials[16]->name = builtin_material_names[449];
  index->groups[12]->materials[16]->data = winter16_iv;
  index->groups[12]->materials[17]->name = builtin_material_names[450];
  index->groups[12]->materials[17]->data = winter17_iv;
  index->groups[12]->materials[18]->name = builtin_material_names[451];
  index->groups[12]->materials[18]->data = winter18_iv;
  index->groups[12]->materials[19]->name = builtin_material_names[452];
  index->groups[12]->materials[19]->data = winter19_iv;
  index->groups[12]->materials[20]->name = builtin_material_names[453];
  index->groups[12]->materials[20]->data = winter20_iv;
  index->groups[12]->materials[21]->name = builtin_material_names[454];
  index->groups[12]->materials[21]->data = winter21_iv;
  index->groups[12]->materials[22]->name = builtin_material_names[455];
  index->groups[12]->materials[22]->data = winter22_iv;
  index->groups[12]->materials[23]->name = builtin_material_names[456];
  index->groups[12]->materials[23]->data = winter23_iv;
  index->groups[12]->materials[24]->name = builtin_material_names[457];
  index->groups[12]->materials[24]->data = winter24_iv;
  index->groups[12]->materials[25]->name = builtin_material_names[458];
  index->groups[12]->materials[25]->data = winter25_iv;
  index->groups[12]->materials[26]->name = builtin_material_names[459];
  index->groups[12]->materials[26]->data = winter26_iv;
  index->groups[12]->materials[27]->name = builtin_material_names[460];
  index->groups[12]->materials[27]->data = winter27_iv;
  index->groups[12]->materials[28]->name = builtin_material_names[461];
  index->groups[12]->materials[28]->data = winter28_iv;
  index->groups[12]->materials[29]->name = builtin_material_names[462];
  index->groups[12]->materials[29]->data = winter29_iv;
  index->groups[12]->materials[30]->name = builtin_material_names[463];
  index->groups[12]->materials[30]->data = winter30_iv;
  index->groups[12]->materials[31]->name = builtin_material_names[464];
  index->groups[12]->materials[31]->data = winter31_iv;
  index->groups[12]->materials[32]->name = builtin_material_names[465];
  index->groups[12]->materials[32]->data = winter32_iv;
  index->groups[12]->materials[33]->name = builtin_material_names[466];
  index->groups[12]->materials[33]->data = winter33_iv;
  index->groups[12]->materials[34]->name = builtin_material_names[467];
  index->groups[12]->materials[34]->data = winter34_iv;
#endif // ! WITH_STATIC_DEFAULTS
} // setupBuiltinMaterials()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoAnyMaterialListBuiltinsRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

