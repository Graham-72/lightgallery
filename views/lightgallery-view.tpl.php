<?php
/**
 * @file
 * Template to display a view as a lightgallery instance.
 */
$rows = $variables['rows'];
$id = $variables['id'];
$field_labels = $variables['field_labels'];
$row_styles = $variables['row_styles'];
$show_more = $variables['show_more'];
?>
<div class="lightgallery-wrapper">
    <ul class="lightgallery" id="lightgallery-<?php print $id; ?>">
        <?php foreach ($rows as $num => $row): ?>
          <li data-src="<?php print $row['slide']; ?>" style="<?php print $row_styles[$num] ?>" data-sub-html="<?php print $row['title']; ?>">
              <?php if (!empty($field_labels[$num])): ?>
                <div class="field-label">
                    <label><?php print $field_labels[$num]; ?></label>
                </div>
              <?php endif; ?>
              <div class="field-content">
                  <a href="">
                      <img class="img-responsive" src="<?php print $row['thumb']; ?>" />
                  </a>
              </div
          </li>
        <?php endforeach; ?>
    </ul>
    <?php if (!empty($show_more)): ?>
      <a href="#" class="show-more" data-unique-id="<?php print $id; ?>" data-open-slide="<?php print $show_more['initial_images']; ?>"><?php print $show_more['label']; ?></a>
    <?php endif; ?>
</div>