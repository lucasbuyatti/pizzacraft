#pragma once
#include "../../../includes.h"

namespace blocks {
    enum class Block {
        Stone,
        Grass,
        Dirt,
        Cobblestone,
        Planks,
        Bedrock,
        Sand,
        Gravel,
        Log,
        Sponge,
        Glass,
        Sandstone,
        Wool,
        BrickBlock,
        Obsidian,
        StainedGlass,
        EndStone,
        StainedHardenedClay,
        Log2,
        RedSandstone,
        QuartzBlock,
        StoneBrick,
        // Agregados por AI
        Clay,
        SnowBlock,
        Ice,
        PackedIce,
        CoalBlock,
        IronBlock,
        GoldBlock,
        DiamondBlock,
        EmeraldBlock,
        LapisBlock,
        RedstoneBlock,
        NetherBrick,
        Glowstone,
        Prismarine,
        SeaLantern,
        HayBlock,
        HardenedClay,
        MelonBlock,
        Pumpkin,
        Netherrack,
        SoulSand,
        Mycelium,
        Bookshelf,
        MossyCobblestone,
        ChiseledSandstone,
        SmoothSandstone,
        ChiseledRedSandstone,
        SmoothRedSandstone,
        CoalOre,
        IronOre,
        GoldOre,
        DiamondOre,
        EmeraldOre,
        LapisOre,
        RedstoneOre
    };

    extern std::unordered_map<std::string, blocks::Block> blockMap;

    bool isUsingBlocks(JNIEnv* env);
}