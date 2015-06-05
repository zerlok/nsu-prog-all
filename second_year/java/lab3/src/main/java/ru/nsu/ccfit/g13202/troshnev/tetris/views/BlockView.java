package ru.nsu.ccfit.g13202.troshnev.tetris.views;

import ru.nsu.ccfit.g13202.troshnev.tetris.kernel.Block;
import ru.nsu.ccfit.g13202.troshnev.tetris.kernel.Coordinate;

import javax.swing.*;
import java.awt.*;

/**
 * Created by zerlok on 4/29/15.
 */

public class BlockView {
    private int pixelWidth;
    private int pixelPadding;
    private int pixelOffset;

    public BlockView(int width, int padding, int margin) {
        pixelWidth = width;
        pixelPadding = padding;
        pixelOffset = width + margin;
    }

    public void draw(Block block, Graphics2D g2d) {
        if (block == null)
            return;

        Coordinate blockPosition = block.getCoordinates();
        int pixelX = blockPosition.getColumnNum() * pixelOffset;
        int pixelY = blockPosition.getRowNum() * pixelOffset;

        Color innerColor = block.getColor();
        Color borderColor = innerColor.brighter();

        g2d.setColor(borderColor);
        g2d.fillRect(
                pixelX,
                pixelY,
                pixelWidth,
                pixelWidth
        );
        g2d.setColor(innerColor);
        g2d.fillRect(
                pixelX + pixelPadding,
                pixelY + pixelPadding,
                pixelWidth - (2 * pixelPadding),
                pixelWidth - (2 * pixelPadding)
        );
    }
}
